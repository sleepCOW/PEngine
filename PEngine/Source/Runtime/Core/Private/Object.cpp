#include "Core/Public/Object.h"
#include "Core/Public/Component.h"
#include "Object/Public/Level.h"

DEFINE_META(CObject)

CObject::CObject(CObject* ThisOwner) : Owner(ThisOwner)
{

}

CObject::~CObject()
{

}

void CObject::PreInit()
{
	ObjectName = typeid(*this).name();
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