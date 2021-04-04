#pragma once

#include "Core/Public/CoreTypes.h"

/** Public interface to FEngineLoop so we can call it from editor or editor code */
class IEngineLoop
{
public:

	// no function calls are available here, this function should only return width, height and fullscreen values
	virtual void PreInit(SWindowParam& OutWindowParam) = 0;

	// return : true - ok, false - failed, application will exit
	virtual bool Init() = 0;

	virtual void Close() = 0;

	// return value: if true will exit the application
	virtual void Tick(float DeltaTime) = 0;
};