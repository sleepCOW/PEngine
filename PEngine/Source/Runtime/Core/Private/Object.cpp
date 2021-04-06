#include "Core/Public/Object.h"
#include "Core/Public/Component.h"

DEFINE_META(CObject)

CObject::CObject(CObject* ThisOwner)
{
	if (ThisOwner)
	{
		Owner = ThisOwner->GetShared();
	}
}

CObject::~CObject()
{

}

void CObject::PreInit()
{

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
	Components.push_back(Component->GetComponentShared());
}

void CObject::AddComponent(SPtr<CComponent> Component)
{
	Components.push_back(Component);
}

SPtr<CObject> CObject::GetShared()
{
	return shared_from_this();
}

SPtr<const CObject> CObject::GetShared() const
{
	return shared_from_this();
}

Vector<SPtr<CComponent>>& CObject::GetComponents()
{
	return Components;
}