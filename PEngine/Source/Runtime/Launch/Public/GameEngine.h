#pragma once

#include "Launch/Public/EngineLoop.h"

/**
 * Implements the game engine (Have no editor UI).
 */
class CGameEngine : public CEngine
{
	DECLARE_CLASS(CGameEngine, CEngine)

public:

	virtual bool Init() override;
};