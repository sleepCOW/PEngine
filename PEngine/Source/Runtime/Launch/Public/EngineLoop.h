#pragma once

#include "Engine/Public/PEngine.h"
#include "Core/Public/CoreTypes.h"
#include "Core/Public/CoreMemory.h"
#include "Core/Public/CoreDefines.h"
#include "Core/Public/ObjectManager.h"
#include "Core/Public/ReflectionManager.h"
#include "Core/Public/ConfReader.h"

typedef union SDL_Event SDL_Event;
class CObjectManager;
class CLevel;

/**
 * Implements the main engine loop.
 * Have common parts of GameEngine and EditorEngine
 */
class CEngine
	: public IEngineLoop
{
	DECLARE_CLASS(CEngine, IEngineLoop)
public:
	CEngine();

	// no function calls are available here, this function should only return width, height and fullscreen values
	virtual void PreInit(SWindowParam& OutWindowParam) override;

	// return : true - ok, false - failed, application will exit
	virtual bool Init() override;

	virtual void Close() override;

	virtual void Tick(float DeltaTime) override;

	// Editor UI implementation should exist only for EditorEngine
	virtual void EditorUI(float DeltaTime) = 0;

	virtual void HandleInput(const SDL_Event& Event);

	/** Access Helpers */
	CObjectManager* GetObjectManager() const;
	CConfReader* GetConfReader() const;
	CLevel* GetLevel() const { return CurrentLevel; }

protected:
	UPtr<CObjectManager> ObjectManager;
	UPtr<CConfReader> ConfReader;

	CLevel* CurrentLevel;
};

int Run(CEngine* EngineLoop);