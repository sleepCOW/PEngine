#pragma once

#include "Component/Public/RenderComponent.h"
#include "SDL2/Public/SDL_rect.h"

class CActor;

/**
 * Component to render single sprite on screen
 */
class COutlineComponent : public CRenderComponent
{
	DECLARE_CLASS(COutlineComponent, CRenderComponent);

public:
	COutlineComponent(CObject* Owner);

	/** Triggers when object is created during runtime (both in editor and in game) */
	virtual void PreInit();

#ifdef WITH_EDITOR
	virtual void FillEditorFields();

	virtual void EditorTick(float DeltaTime) override;
#endif

	/** Object tick, called only in runtime */
	virtual void Tick(float DeltaTime);

	virtual void Draw() const override;

	SDL_Rect& GetRectangle() { return OutlineRectangle; }

protected:
	CActor* ActorOwner;
	SDL_Rect OutlineRectangle;
};