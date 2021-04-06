#ifdef WITH_EDITOR
#pragma once

#include "CoreMinimal.h"

class CReflectionManager
{
public:

	void AddClass(const String& ClassName);

protected:
	Vector<String> Classes;
};

class CReflectionData
{
public:
	CReflectionData(const String& ClassName);
};
#endif