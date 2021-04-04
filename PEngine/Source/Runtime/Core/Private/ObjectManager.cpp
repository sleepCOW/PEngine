#include "Core/Public/ObjectManager.h"

void CObjectManager::AddObject(CObject* NewObject)
{
	Objects.push_back(std::make_shared<CObject>(NewObject));
}

void CObjectManager::AddObject(SPtr<CObject> NewObject)
{
	Objects.push_back(NewObject);
}

Vector<SPtr<CObject>>& CObjectManager::GetObjects()
{
	return Objects;
}
