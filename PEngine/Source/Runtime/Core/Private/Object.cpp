#include "Core/Public/Object.h"
#include "Core/Public/Component.h"
#include "Object/Public/Level.h"

DEFINE_META(CObject)

CObject::CObject(CObject* ThisOwner) : Owner(ThisOwner)
{
	bTicking = true;
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