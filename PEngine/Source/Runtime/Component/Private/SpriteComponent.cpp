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

bool CSpriteComponent::Serialize(rapidjson::Value& OutValue, SArchive& Archive)
{
	Super::Serialize(OutValue, Archive);

	using rapidjson::Value;

	auto& Allocator = Archive.GetAllocator();

	OutValue.AddMember("TexturePath", Value(TexturePath, Allocator), Allocator);
	OutValue.AddMember("Scale_X", Value(Scale.X), Allocator);
	OutValue.AddMember("Scale_Y", Value(Scale.Y), Allocator);
	OutValue.AddMember("ZOrder", Value(ZOrder), Allocator);

	return true;
}

bool CSpriteComponent::Deserialize(rapidjson::Value& InValue, SArchive& Archive)
{
	Super::Deserialize(InValue, Archive);

	TexturePath = InValue["TexturePath"].GetString();
	Scale.X = InValue["Scale_X"].GetFloat();
	Scale.Y = InValue["Scale_Y"].GetFloat();
	ZOrder = InValue["ZOrder"].GetInt();

	Sprite.AssignNewTexture(TexturePath);

	return true;
}

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
