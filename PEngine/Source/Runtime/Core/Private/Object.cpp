#include "Core/Public/Object.h"
#include "Core/Public/Component.h"
#include "Object/Public/Level.h"

DEFINE_META(CObject)

using namespace rapidjson;

CObject::CObject(CObject* ThisOwner) : Owner(ThisOwner)
{
	bTicking = true;
	bMustBeSerialized = true;
}

CObject::~CObject()
{
	for (auto& Component : Components)
	{
		delete Component;
	}
}

void CObject::PreInit()
{
	ObjectName = GetClassName();
}

void CObject::Init()
{

}

void CObject::Tick(float DeltaTime)
{
	for (auto& Component : GetComponents())
	{
		if (Component->bTicking)
		{
			Component->Tick(DeltaTime);
		}
	}
}

bool CObject::Serialize(rapidjson::Value& OutValue, SArchive& Archive)
{
	auto& Allocator = Archive.GetAllocator();

	OutValue.SetObject();
	OutValue.AddMember("ObjectIndex", Archive.GetObjectIndex(this), Allocator);

	Value ClassNameValue(GetClassName(), Allocator);
	OutValue.AddMember("ObjectType", ClassNameValue, Allocator);
	
	OutValue.AddMember("ObjectOwnerIndex", Archive.GetObjectIndex(Owner), Allocator);

	Value ObjectNameValue(ObjectName, Allocator);
	OutValue.AddMember(TO_STR(ObjectName), ObjectNameValue, Allocator);

	Value ComponentArray(kArrayType);
	for (auto& Component : Components)
	{
		if (!Component->bMustBeSerialized)
			continue;

		Value ComponentValue;
		Component->Serialize(ComponentValue, Archive);
		ComponentArray.PushBack(ComponentValue, Allocator);
	}
	OutValue.AddMember(TO_STR(Components), ComponentArray, Allocator);

	return true;
}

bool CObject::Deserialize(rapidjson::Value& InValue, SArchive& Archive)
{
	// register this object pointer within archive
	Archive.AddObjectIndex(this, InValue["ObjectIndex"].GetInt());

	int ObjectOwnerIndex = InValue["ObjectOwnerIndex"].GetInt();
	Owner = Archive.GetObjectFromIndex(ObjectOwnerIndex);

	for (auto& Component : InValue[TO_STR(Components)].GetArray())
	{
		String ObjectType = Component["ObjectType"].GetString();
		CObject* NewComponent = GReflectionManager.CreateObject(ObjectType, this);
		NewComponent->Deserialize(Component, Archive);
	}

	return true;
}

void CObject::AddComponent(CComponent* Component)
{
	assert(Component != nullptr);

	Components.push_back(Component);
}

Vector<CComponent*>& CObject::GetComponents()
{
	return Components;
}

#ifdef WITH_EDITOR
void CObject::FillEditorFields()
{
	AddEditorField("Object name", &ObjectName, EFieldType::STRING);
}

void CObject::AddEditorField(const SField& Field)
{
	EditorFields.push_back(Field);
}

void CObject::AddEditorField(const char* FieldName, void* FieldAdress, EFieldType FieldType)
{
	AddEditorField({ FieldAdress, FieldName, FieldType });
}

void CObject::EditorTick(float DeltaTime)
{
	for (auto& Component : GetComponents())
	{
		Component->EditorTick(DeltaTime);
	}
}

void CObject::PostEditChangeProperty(SField& ChangedValue)
{

}

#endif