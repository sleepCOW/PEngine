#pragma once

#include "Core/Public/Object.h"

class CObjectManager
{
public:
	~CObjectManager() {}

	void AddObject(CObject* NewObject);
	void AddObject(SPtr<CObject> NewObject);

	Vector<SPtr<CObject>>& GetObjects();

protected:
	Vector<SPtr<CObject>> Objects;
};