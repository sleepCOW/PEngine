#pragma once

#include "Launch/Public/EngineLoop.h"

/**
 * Implements the game engine (Have no editor UI).
 */
class CGameEngine : public CEngine
{
public:

	// Editor UI implementation should exist only for EditorEngine
	virtual void EditorUI(float DeltaTime) override;
};