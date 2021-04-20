#include "Core/Public/Component.h"

CComponent::CComponent(CObject* Owner) : Super(Owner)
{
	bTicking = true;
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
	Super::Tick(DeltaTime);
}

#ifdef WITH_EDITOR
void CComponent::EditorTick(float DeltaTime)
{
	Super::EditorTick(DeltaTime);
}
#endif WITH_EDITOR