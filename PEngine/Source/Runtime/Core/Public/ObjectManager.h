#pragma once

#include "Core/Public/Object.h"

class CRenderComponent;

class CObjectManager
{
public:
	~CObjectManager() {}

	void AddObject(CObject* NewObject);
	void AddObject(SPtr<CObject> NewObject);
	void AddRenderComponent(CRenderComponent* NewObject);
	void AddRenderComponent(SPtr<CRenderComponent> NewObject);

	Vector<SPtr<CObject>>& GetObjects();
	List<SPtr<CRenderComponent>>& GetRenderComponents();

protected:
	Vector<SPtr<CObject>> Objects;
	List<SPtr<CRenderComponent>> RenderComponents;
};