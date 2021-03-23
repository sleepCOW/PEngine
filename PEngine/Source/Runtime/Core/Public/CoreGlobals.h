#pragma once

#include "Core/Public/CoreTypes.h"
#include "windows.h"

extern struct SDL_Renderer* GRenderer;
extern struct SDL_Window* GMainWindow;

extern bool GIsRequestingExit;
extern bool GEngineInitialized;

FORCEINLINE bool IsEngineExitRequested()
{
	return GIsRequestingExit;
}