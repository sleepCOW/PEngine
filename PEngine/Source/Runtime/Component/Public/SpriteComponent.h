#pragma once

#include "Component/Public/RenderComponent.h"
#include "Core/Public/AssetManager.h"

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

	virtual void PostEditChangeProperty(SField& ChangedValue) override;
#endif

	virtual void Draw() const override;

protected:
	String TexturePath;

	SSprite Sprite;
	SVector Scale;

	int ZOrder;
};