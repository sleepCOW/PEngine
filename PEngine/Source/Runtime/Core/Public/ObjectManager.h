#pragma once

#include "CoreMemory.h"

class CObject;
class CRenderComponent;

class CObjectManager
{
public:
	~CObjectManager() {}

	void AddObject(CObject* NewObject);
	void AddRenderComponent(CRenderComponent* NewObject);

	Vector<CObject*>& GetObjects();
	List<CRenderComponent*>& GetRenderComponents();

protected:
	Vector<CObject*> Objects;
	List<CRenderComponent*> RenderComponents;
};