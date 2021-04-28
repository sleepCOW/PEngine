#include "Component/Public/OutlineComponent.h"
#include "Object/Public/Level.h"
#include "SDL2/Public/SDL_render.h"
#include "Core/Public/Actor.h"

DEFINE_META(COutlineComponent)

COutlineComponent::COutlineComponent(CObject* Owner) : Super(Owner)
{

}

void COutlineComponent::PreInit()
{
	Super::PreInit();

	ActorOwner = dynamic_cast<CActor*>(Owner);
	OutlineRectangle = SDL_Rect{ 0, 0, 100, 100 };
}

#ifdef WITH_EDITOR
void COutlineComponent::FillEditorFields()
{
	Super::FillEditorFields();

	AddEditorField("Outline Rectangle", &OutlineRectangle, EFieldType::RECTANGLE);
}

void COutlineComponent::EditorTick(float DeltaTime)
{
	Super::EditorTick(DeltaTime);

	OutlineRectangle.x = ActorOwner->GetLocation().X;
	OutlineRectangle.y = ActorOwner->GetLocation().Y;
}
#endif

void COutlineComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void COutlineComponent::Draw() const
{
	SDL_SetRenderDrawColor(GRenderer, 0, 0, 255, 255);
	SDL_RenderDrawRect(GRenderer, &OutlineRectangle);
}