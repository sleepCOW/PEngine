#ifdef WITH_EDITOR
#pragma once

#include "CoreMinimal.h"

using NewObjectFunc = CObject* (*)(CObject*);
using NewActorFunc = void(*)(CLevel*);

class CReflectionManager
{
public:

	void AddObject(const String& ClassName);
	void AddComponent(const String& ClassName);
	void AddToTypeMap(const String& ClassName, NewObjectFunc Function);
	void AddToTypeMap(const String& ClassName, NewActorFunc Function);

	Vector<String>& GetObjects();
	Vector<String>& GetComponents();

	CObject* CreateObject(const String& ObjectName, CObject* Owner);
	void CreateActor(const String& ObjectName, CLevel* Owner);


protected:
	// All registered objects and components
	Vector<String> Objects;
	Vector<String> Components;

	// Map Type -> CreateFunction instance to create instances via string name
	Map<String, NewObjectFunc> ObjectTypeMap;
	Map<String, NewActorFunc> ActorTypeMap;
};

class CReflectionData
{
public:
	CReflectionData(const String& ClassName, bool bComponent, NewObjectFunc Function, NewActorFunc ActorFunction);
};
#endif