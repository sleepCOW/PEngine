#pragma once

#include "Core/Public/CoreMinimal.h"
#include "Engine/Public/PEngine.h"

/**
 * Implements the main engine loop.
 * Have common parts of GameEngine and EditorEngine
 */
class CEngine
	: public IEngineLoop
{
	DECLARE_CLASS(CEngine, IEngineLoop)
public:

	// no function calls are available here, this function should only return width, height and fullscreen values
	virtual void PreInit(SWindowParam& OutWindowParam) override;

	// return : true - ok, false - failed, application will exit
	virtual bool Init() override;

	virtual void Close() override;

	virtual void Tick(float DeltaTime) override;

	// Editor UI implementation should exist only for EditorEngine
	virtual void EditorUI(float DeltaTime) = 0;

	virtual void HandleInput(const SDL_Event& Event);
};

int Run(CEngine* EngineLoop);

/** Global engine loop object. */
extern CEngine* GEngineLoop;
