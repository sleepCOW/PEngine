#include "Component/Public/SpriteComponent.h"
#include "Object/Public/Level.h"
#include "Core/Public/Actor.h"

DEFINE_META(CSpriteComponent)

CSpriteComponent::CSpriteComponent(CObject* Owner) : Super(Owner)
{
	Scale = { 1.f, 1.f };
	Scale.DragSpeed = 0.01f;
}

#ifdef WITH_EDITOR
void CSpriteComponent::FillEditorFields()
{
	Super::FillEditorFields();

	AddEditorField("Texture Path", &TexturePath, EFieldType::STRING);
	AddEditorField("Scale", &Scale, EFieldType::MATH_VECTOR);
}

void CSpriteComponent::PostEditChangeProperty(SField& ChangedValue)
{
	Super::PostEditChangeProperty(ChangedValue);

	if (ChangedValue.FieldName == "Texture Path" && ChangedValue.FieldType == EFieldType::STRING)
	{
		String* Field = reinterpret_cast<String*>(ChangedValue.PField);

		Sprite.AssignNewTexture(*Field);
	}
}

#endif

void CSpriteComponent::Draw() const
{
	if (Sprite.Texture)
	{
		SDL_Rect Source = { 0, 0, Sprite.Width, Sprite.Height };
		SVector OwnerLoc = ActorOwner->GetLocation();
		SDL_Rect Target = { OwnerLoc.X, OwnerLoc.Y, Sprite.Width * Scale.X, Sprite.Height * Scale.Y};
		SDL_RenderCopy(GRenderer, Sprite.Texture, &Source, &Target);
	}
}
