#include "Core/Public/ObjectManager.h"
#include "Core/Public/Object.h"
#include "Component/Public/RenderComponent.h"

void CObjectManager::AddObject(CObject* NewObject)
{
	assert(NewObject);

	Objects.push_back(NewObject);
}

void CObjectManager::AddRenderComponent(CRenderComponent* NewObject)
{
	assert(NewObject);

	RenderComponents.push_back(NewObject);
}

Vector<CObject*>& CObjectManager::GetObjects()
{
	return Objects;
}

List<CRenderComponent*>& CObjectManager::GetRenderComponents()
{
	return RenderComponents;
}
