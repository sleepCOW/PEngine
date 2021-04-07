#ifdef WITH_EDITOR
#pragma once

#include "CoreMinimal.h"

class CReflectionManager
{
public:

	void AddObject(const String& ClassName);
	void AddComponent(const String& ClassName);

protected:
	// All registered objects and components
	Vector<String> Objects;
	Vector<String> Components;
};

class CReflectionData
{
public:
	CReflectionData(const String& ClassName, bool bComponent);
};
#endif