#ifdef WITH_EDITOR
#include "Core/Public/ReflectionManager.h"


void CReflectionManager::AddObject(const String& ClassName)
{
	// Check whether this class already registered
	auto It = find(begin(Objects), end(Objects), ClassName);
	assert(It == Objects.end());

	Objects.push_back(std::move(ClassName));
}

void CReflectionManager::AddComponent(const String& ClassName)
{
	// Check whether this class already registered
	auto It = find(begin(Components), end(Components), ClassName);
	assert(It == Components.end());

	Components.push_back(std::move(ClassName));
}

CReflectionData::CReflectionData(const String& ClassName, bool bComponent)
{
	if (bComponent)
	{
		ReflectionManager.AddComponent(ClassName);
	}
	else
	{
		ReflectionManager.AddObject(ClassName);
	}
}
#endif