#include "Object/Public/Level.h"
#include "Core/Public/ConfReader.h"
#include "Launch/Public/EngineLoop.h"

CLevel::CLevel(CObject* ThisOwner) : Super(ThisOwner)
{
	LevelJSON = nullptr;
}

CLevel::~CLevel()
{
	for (auto& Object : LevelObjects)
	{
		delete Object;
	}
}

void CLevel::RegisterObject(CObject* NewObject)
{
	LevelObjects.push_back(NewObject);
}

void CLevel::NewLevel()
{
	if (!LevelJSON)
	{
		LevelJSON = new rapidjson::Document;
	}

	LevelName = "untitled";
	LevelJSON->Clear();
}

void CLevel::LoadFromJson(const String& FileName)
{
	LevelJSON = &(GEngineLoop->GetConfReader()->OpenJSON(FileName, true)->Json);
	LevelName = FileName;
	
	// #TODO Deserialize objects in json
}

void CLevel::PreInit()
{
	Super::PreInit();
}

void CLevel::Init()
{
	Super::Init();

	for (auto& Object : LevelObjects)
	{
		Object->Init();
	}
}

void CLevel::Tick(float DeltaTime)
{
	for (auto& Object : LevelObjects)
	{
		if (Object->bTicking)
		{
			Object->Tick(DeltaTime);
		}
	}
}

