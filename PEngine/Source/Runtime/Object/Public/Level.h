#pragma once

#include "Core/Public/CoreMinimal.h"
#include "Core/Public/Object.h"

class CLevel : public CObject
{
	DECLARE_CLASS(CLevel, CObject)
public:
	CLevel(CObject* ThisOwner = nullptr);
	~CLevel();

	/** Triggers when object is created during runtime */
	virtual void PreInit();

	/** Triggers when game started */
	virtual void Init();

	/** Object tick, called only in runtime */
	virtual void Tick(float DeltaTime);

	/** Helpers */
	Vector<CObject*>& GetObjects() { return LevelObjects; }
	const String& GetName() { return LevelName; }

	void RegisterObject(CObject* NewObject);

	/**
	 * Clear state & create clean level
	 */
	void NewLevel();
	void LoadFromJson(const String& FileName);

	/**
	 * Create object that is owned by this level
	 */
	template <typename T>
	CObject* SpawnObject()
	{
		CObject* CreatedObject = new T(this);

		LevelObjects.push_back(CreatedObject);

		CreatedObject->PreInit();

		return CreatedObject;
	}

protected:
	Vector<CObject*> LevelObjects;

	String LevelName;
	rapidjson::Document* LevelJSON;
};

#ifdef WITH_EDITOR

/**
* Create object that is owned by this level
*/
template <typename T>
void SpawnActor(CLevel* Level)
{
	assert(Level);
	Level->SpawnObject<T>();
}
#endif