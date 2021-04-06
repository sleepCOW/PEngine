#include "Core/Public/Component.h"

DEFINE_META(CComponent)

CComponent::CComponent(CObject* Owner) : Super(Owner)
{
}

void CComponent::PreInit()
{

}

void CComponent::Init()
{

}

void CComponent::Tick(float DeltaTime)
{

}

SPtr<CComponent> CComponent::GetComponentShared()
{
	return std::static_pointer_cast<CComponent>(GetShared());
}

SPtr<const CComponent> CComponent::GetComponentShared() const
{
	return std::static_pointer_cast<const CComponent>(GetShared());
}