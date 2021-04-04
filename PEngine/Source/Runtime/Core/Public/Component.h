#pragma once

#include "Core/Public/Object.h"

class CComponent : public CObject
{
	DECLARE_CLASS(CComponent, CObject)

public:
	CComponent(CObject* Owner);
	~CComponent() {}

	/** Whether this component active and ticking */
	bool bActive;

protected:

};