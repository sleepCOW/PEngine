#include "Core/Public/ObjectManager.h"
#include "Core/Public/Object.h"
#include "Component/Public/RenderComponent.h"

void CObjectManager::AddObject(CObject* NewObject)
{
	Objects.push_back(std::make_shared<CObject>(NewObject));
}

void CObjectManager::AddObject(SPtr<CObject> NewObject)
{
	Objects.push_back(NewObject);
}

void CObjectManager::AddRenderComponent(CRenderComponent* NewObject)
{
	RenderComponents.push_back(std::static_pointer_cast<CRenderComponent>(NewObject->GetShared()));
}

void CObjectManager::AddRenderComponent(SPtr<CRenderComponent> NewObject)
{
	RenderComponents.push_back(NewObject);
}

Vector<SPtr<CObject>>& CObjectManager::GetObjects()
{
	return Objects;
}

List<SPtr<CRenderComponent>>& CObjectManager::GetRenderComponents()
{
	return RenderComponents;
}
