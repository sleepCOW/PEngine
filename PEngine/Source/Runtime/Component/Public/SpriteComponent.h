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


protected:
	int ZOrder;
};