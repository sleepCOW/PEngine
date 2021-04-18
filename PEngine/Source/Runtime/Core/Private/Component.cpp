#include "Core/Public/Component.h"

CComponent::CComponent(CObject* Owner) : Super(Owner)
{
}

void CComponent::PreInit()
{
	Super::PreInit();
}

void CComponent::Init()
{
	Super::Init();
}

void CComponent::Tick(float DeltaTime)
{

}