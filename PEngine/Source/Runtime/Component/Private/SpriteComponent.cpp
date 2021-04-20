#include "Component/Public/SpriteComponent.h"
#include "Object/Public/Level.h"

DEFINE_META(CSpriteComponent)

CSpriteComponent::CSpriteComponent(CObject* Owner) : Super(Owner)
{

}

#ifdef WITH_EDITOR
void CSpriteComponent::FillEditorFields()
{
	Super::FillEditorFields();
}
#endif

void CSpriteComponent::Draw() const
{

}
