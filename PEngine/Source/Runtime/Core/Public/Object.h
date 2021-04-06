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

	void AddComponent(CComponent* Component);
	void AddComponent(SPtr<CComponent> Component);

	/** Helper functions */
	SPtr<CObject> GetShared();
	SPtr<const CObject> GetShared() const;
	Vector<SPtr<CComponent>>& GetComponents();

	/** Whether this object is active and ticking */
	bool bActive;

protected:
	/** Owner of this component */
	WeakPtr<CObject> Owner;

	Vector<SPtr<CComponent>> Components;

};