#pragma once

#include "Launch/Public/EngineLoop.h"

/**
 * Implements editor engine (have addional UI layer)
 */
class CEditorEngine : public CEngine 
{
	DECLARE_CLASS(CEditorEngine, CEngine)

public:

	virtual bool Init() override;

	virtual void Close() override;

	virtual void Tick(float DeltaTime) override;

	// Editor UI implementation should exist only for EditorEngine
	virtual void EditorUI(float DeltaTime) override;

	virtual void HandleInput(const SDL_Event& Event) override;
};