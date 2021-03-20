#pragma once

#include "Core/Public/CoreTypes.h"
#include "windows.h"

extern bool GIsRequestingExit;
extern bool GEngineInitialized;

FORCEINLINE bool IsEngineExitRequested()
{
	return GIsRequestingExit;
}