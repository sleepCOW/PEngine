#pragma once

#include "CoreMinimal.h"

class CComponent;

/**
 * Base class for ?
 */
class CObject : std::enable_shared_from_this<CObject>
{
	DECLARE_CLASS(CObject, CObject)

public:
	CObject(CObject* ThisOwner);
	virtual ~CObject();

	/** Triggers when object is created during runtime */
	virtual void PreInit();

	/** Triggers when game started */
	virtual void Init();

	/** Object tick, called only in runtime */
	virtual void Tick(float DeltaTime);

	/** Helper functions */
	SPtr<CObject> GetShared();
	SPtr<const CObject> GetShared() const;

protected:
	/** Owner of this component */
	WeakPtr<CObject> Owner;

	Vector<SPtr<CComponent>> Components;
};