#include "Core/Public/Actor.h"
#include "Object/Public/Level.h"

DEFINE_META(CActor)

CActor::CActor(CObject* ThisOwner /*= nullptr*/) : Super(ThisOwner)
{

}

void CActor::FillEditorFields()
{
	Super::FillEditorFields();

	EditorFields.push_back({ &Location, EFieldType::MATH_VECTOR});
}
