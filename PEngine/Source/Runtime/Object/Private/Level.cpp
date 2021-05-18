#include "Object/Public/Level.h"
#include "Core/Public/ConfReader.h"
#include "Launch/Public/EngineLoop.h"

using namespace rapidjson;

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
	// Shall we set objects owner to be this world, as well?
	LevelObjects.push_back(NewObject);
}

void CLevel::NewLevel()
{
	LevelName = "Unnamed.json";
	LevelJSON = &(GEngine->GetConfReader()->OpenJSON(LevelName, true)->Json);
}

void CLevel::LoadFromJson(const String& FileName)
{
	LevelJSON = &(GEngine->GetConfReader()->OpenJSON(FileName, true)->Json);
	LevelName = FileName;

	// Data is corrupted or empty
	if (LevelJSON->HasParseError() || LevelJSON->Empty() || !LevelJSON->IsArray())
	{
		// Say some error
		return;
	}
	
	// #TODO Deserialize objects in json
	SArchive Archive(LevelJSON->GetAllocator());
	// Level is always have first index
	Archive.AddObjectIndex(this, 1);
	for (auto& Object : LevelJSON->GetArray())
	{
		assert(Object.IsObject());

		String ObjectType = Object["ObjectType"].GetString();
		CObject* NewObject = GReflectionManager.CreateObject(ObjectType, this);
		RegisterObject(NewObject);
		NewObject->Deserialize(Object, Archive);
	}
}

void CLevel::SaveToJson()
{
	// #TODO Clear file before filling it again to prevent 2 copies of the data in the file!
	LevelJSON->SetArray();
	LevelJSON->Clear();

	SArchive Archive(LevelJSON->GetAllocator());
	for (auto& Object : LevelObjects)
	{
		if (!Object->bMustBeSerialized)
			continue;

		Value ObjectValue;
		Object->Serialize(ObjectValue, Archive);
		LevelJSON->GetArray().PushBack(ObjectValue, LevelJSON->GetAllocator());
	}
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

#ifdef WITH_EDITOR
void CLevel::EditorTick(float DeltaTime)
{
	Super::EditorTick(DeltaTime);

	for (auto& Object : LevelObjects)
	{
		Object->EditorTick(DeltaTime);
	}
}
#endif
