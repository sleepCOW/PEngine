#ifdef WITH_EDITOR
#include "Core/Public/ReflectionManager.h"


void CReflectionManager::AddClass(const String& ClassName)
{
	// Check whether this class already registered
	auto It = find(begin(Classes), end(Classes), ClassName);
	assert(It == Classes.end());

	Classes.push_back(std::move(ClassName));
}

CReflectionData::CReflectionData(const String& ClassName)
{
	ReflectionManager.AddClass(ClassName);
}
#endif