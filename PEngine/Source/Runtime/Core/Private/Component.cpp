#include "Core/Public/Component.h"
#include "Core/Public/Actor.h"

CComponent::CComponent(CObject* Owner) : Super(Owner)
{
	bTicking = true;

	ActorOwner = dynamic_cast<CActor*>(Owner);
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

bool CComponent::Serialize(rapidjson::Value& OutValue, SArchive& Archive)
{
	return Super::Serialize(OutValue, Archive);
}

#ifdef WITH_EDITOR
void CComponent::EditorTick(float DeltaTime)
{
	Super::EditorTick(DeltaTime);
}
#endif WITH_EDITOR