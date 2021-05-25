#include "Component/Public/OutlineComponent.h"
#include "Object/Public/Level.h"
#include "SDL2/Public/SDL_render.h"
#include "Core/Public/Actor.h"
#include "Component/Public/SpriteComponent.h"

DEFINE_META(COutlineComponent)

COutlineComponent::COutlineComponent(CObject* Owner) : Super(Owner)
{
	bMustBeSerialized = false;
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

	CSpriteComponent* OwnerSprite = ActorOwner->GetComponentByClass<CSpriteComponent>();
	if (OwnerSprite && OwnerSprite->GetSprite().Texture)
	{
		OutlineRectangle.w = OwnerSprite->GetSprite().Width * OwnerSprite->GetScale().X;
		OutlineRectangle.h = OwnerSprite->GetSprite().Height * OwnerSprite->GetScale().Y;
	}
}
#endif

void COutlineComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void COutlineComponent::Draw() const
{
	if (GEngine->GetGamePaused())
	{
		SDL_SetRenderDrawColor(GRenderer, 0, 0, 255, 255);
		SDL_RenderDrawRect(GRenderer, &OutlineRectangle);
	}
}