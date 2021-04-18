#pragma once

#include "CoreMinimal.h"
#include "Core/Public/Object.h"

/**
 * Class that has location on scene
 */
class CActor : public CObject
{
	DECLARE_CLASS(CActor, CObject)

public:
	CActor(CObject* ThisOwner = nullptr);

protected:
};