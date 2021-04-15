#pragma once

#include "CoreMinimal.h"
#include "Launch/Public/EngineLoop.h"

class CComponent;
class CRenderComponent;

/**
 * Base class for ?
 */
class CObject
{
	DECLARE_CLASS(CObject, CObject)

public:
	CObject(CObject* ThisOwner = nullptr);
	virtual ~CObject();

	/** Triggers when object is created during runtime */
	virtual void PreInit();

	/** Triggers when game started */
	virtual void Init();

	/** Object tick, called only in runtime */
	virtual void Tick(float DeltaTime);

	void AddComponent(CComponent* Component);

	/** Helper functions */
	Vector<CComponent*>& GetComponents();

	/** Whether this object is active and ticking */
	bool bActive;

protected:
	/** Owner of this component */
	CObject* Owner;

	Vector<CComponent*> Components;
};

// Create new object in runtime/editor
template <typename T>
CObject* NewObject(CObject* Owner)
{
	CObjectManager* ObjectManager = GEngineLoop->GetObjectManager();

	CObject* CreatedObject = new T(Owner);

	bool bIsObject = false;
	if constexpr (std::is_base_of<CComponent, T>::value)
	{
		if constexpr (std::is_base_of<CRenderComponent, T>::value)
		{
			ObjectManager->AddRenderComponent(static_cast<CRenderComponent*>(CreatedObject));
		}

		if (Owner)
		{
			Owner->AddComponent(static_cast<CComponent*>(CreatedObject));
		}
	}
	else
	{
		bIsObject = true;
	}

	CreatedObject->PreInit();

	if (bIsObject)
	{
		ObjectManager->AddObject(CreatedObject);
	}

	return CreatedObject;
}