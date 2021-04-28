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

void CActor::FillEditorFields()
{
	Super::FillEditorFields();

	AddEditorField("Location", &Location, EFieldType::MATH_VECTOR);
}
