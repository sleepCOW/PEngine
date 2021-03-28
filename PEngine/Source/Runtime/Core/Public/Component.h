#pragma once

#include "Core/Public/CoreMinimal.h"

class CComponent
{
public:
	CComponent(CObject* Owner);

protected:

	/** Owner of this component */
	WeakPtr<CObject> Owner;
};