#include "Core/Public/Object.h"
#include "Core/Public/Component.h"
#include "Object/Public/Level.h"

DEFINE_META(CObject)

CObject::CObject(CObject* ThisOwner) : Owner(ThisOwner)
{

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
		if (Component->bActive)
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
	EditorFields.push_back({ &ObjectName, EFieldType::STRING });
}
#endif