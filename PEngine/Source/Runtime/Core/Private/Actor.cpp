#include "Core/Public/Actor.h"
#include "Object/Public/Level.h"
#include "Component/Public/OutlineComponent.h"

DEFINE_META(CActor)

CActor::CActor(CObject* ThisOwner /*= nullptr*/) : Super(ThisOwner)
{

}

void CActor::PreInit()
{
	Super::PreInit();

#ifdef WITH_EDITOR
	NewObject<COutlineComponent>(this);
#endif
}

void CActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool CActor::Serialize(rapidjson::Value& OutValue, SArchive& Archive)
{
	using namespace rapidjson;

	Super::Serialize(OutValue, Archive);

	OutValue.AddMember("Location_X", Value(Location.X), Archive.GetAllocator());
	OutValue.AddMember("Location_Y", Value(Location.Y), Archive.GetAllocator());

	return true;
}

bool CActor::Deserialize(rapidjson::Value& InValue, SArchive& Archive)
{
	Super::Deserialize(InValue, Archive);

	Location.X = InValue["Location_X"].GetFloat();
	Location.Y = InValue["Location_Y"].GetFloat();

	return true;
}

void CActor::FillEditorFields()
{
	Super::FillEditorFields();

	AddEditorField("Location", &Location, EFieldType::MATH_VECTOR);
}
