#pragma once

#include "Core/Public/Object.h"

class CComponent : public CObject
{
	DECLARE_CLASS(CComponent, CObject)

public:
	CComponent(CObject* Owner);
	~CComponent() {}

	/** Triggers when object is created during runtime */
	virtual void PreInit();

	/** Triggers when game started */
	virtual void Init();

	/** Object tick, called only in runtime */
	virtual void Tick(float DeltaTime);

	SPtr<CComponent> GetComponentShared();
	SPtr<const CComponent> GetComponentShared() const;

protected:

};