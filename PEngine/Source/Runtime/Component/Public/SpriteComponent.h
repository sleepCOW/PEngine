#pragma once

#include "Component/Public/RenderComponent.h"

/**
 * Component to render single sprite on screen
 */
class CSpriteComponent : public CRenderComponent
{
	DECLARE_CLASS(CSpriteComponent, CRenderComponent);

public:
	CSpriteComponent(CObject* Owner);

#ifdef WITH_EDITOR
	virtual void FillEditorFields();
#endif

	virtual void Draw() const override;

protected:
	int ZOrder;
};