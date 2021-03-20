#pragma once

#include "Core/Public/CoreMinimal.h"
#include "Engine/Public/PEngine.h"

/**
 * Implements the main engine loop.
 */
class CEngineLoop
	: public IEngineLoop
{
public:

	// no function calls are available here, this function should only return width, height and fullscreen values
	virtual void PreInit(SWindowParam& OutWindowParam) override;

	// return : true - ok, false - failed, application will exit
	virtual bool Init() override;

	virtual void Close() override;

	virtual void Tick(float DeltaTime) override;
};

int Run(IEngineLoop* EngineLoop);

/** Global engine loop object. */
extern CEngineLoop* GEngineLoop;
