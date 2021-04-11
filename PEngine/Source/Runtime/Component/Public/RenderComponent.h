#pragma once

#include "Core/Public/CoreMinimal.h"
#include "Core/Public/Component.h"

class CRenderComponent : public CComponent
{
	DECLARE_CLASS(CRenderComponent, CComponent)

public:

	/** Triggers when object is created during runtime (both in editor and in game) */
	virtual void PreInit();

	/** Triggers when game started */
	virtual void Init();

	/** Object tick, called only in runtime */
	virtual void Tick(float DeltaTime);

	/** All draw calls must be inside of this function */
	virtual void Draw() const;
};