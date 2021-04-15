#ifdef WITH_EDITOR
#include "Core/Public/ReflectionManager.h"

void CReflectionManager::AddObject(const String& ClassName)
{
	// Check whether this class already registered
	auto It = find(begin(Objects), end(Objects), ClassName);
	assert(It == Objects.end());

	Objects.push_back(ClassName);
}

void CReflectionManager::AddComponent(const String& ClassName)
{
	// Check whether this class already registered
	auto It = find(begin(Components), end(Components), ClassName);
	assert(It == Components.end());

	Components.push_back(ClassName);
}

void CReflectionManager::AddToTypeMap(const String& ClassName, NewObjectFunc Function)
{
	// This function must not be called twice for single class!
	assert(ObjectTypeMap.find(ClassName) == ObjectTypeMap.end());

	ObjectTypeMap[ClassName] = Function;
}

Vector<String>& CReflectionManager::GetObjects()
{
	return Objects;
}

Vector<String>& CReflectionManager::GetComponents()
{
	return Components;
}

CObject* CReflectionManager::CreateObject(const String& ObjectName, CObject* Owner)
{
	assert(ObjectTypeMap.find(ObjectName) != ObjectTypeMap.end());

	return ObjectTypeMap[ObjectName](Owner);
}

CReflectionData::CReflectionData(const String& ClassName, bool bComponent, NewObjectFunc Function)
{
	if (bComponent)
	{
		ReflectionManager.AddComponent(ClassName);
	}
	else
	{
		ReflectionManager.AddObject(ClassName);
	}

	ReflectionManager.AddToTypeMap(ClassName, Function);
}

#endif