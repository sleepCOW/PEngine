#include "Component/Public/RenderComponent.h"
#include "Object/Public/Level.h"

DEFINE_META(CRenderComponent)

CRenderComponent::CRenderComponent(CObject* Owner) : Super(Owner)
{

}

void CRenderComponent::PreInit()
{
	Super::PreInit();
}

void CRenderComponent::Init()
{
	Super::Init();
}

void CRenderComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void CRenderComponent::Draw() const
{

}