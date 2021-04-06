#pragma once

#include "Core/Public/CoreTypes.h"
#include "windows.h"

/** Global engine loop object. */
extern struct SDL_Renderer* GRenderer;
extern struct SDL_Window* GMainWindow;

extern bool GIsRequestingExit;
extern bool GEngineInitialized;

#ifdef WITH_EDITOR
extern class CReflectionManager ReflectionManager;
#endif

FORCEINLINE bool IsEngineExitRequested()
{
	return GIsRequestingExit;
}