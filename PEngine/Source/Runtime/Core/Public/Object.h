#pragma once

#include "Launch/Public/EngineLoop.h"
#include "rapidJSON/document.h"

class CComponent;
class CRenderComponent;

struct SArchive
{
public:
	SArchive(rapidjson::Document::AllocatorType& InAllocator) : CurrentObjectIndex(0), JsonAllocator(InAllocator) {}

	int GetObjectIndex(CObject* Object)
	{
		if (ObjectToIndex.find(Object) == ObjectToIndex.end())
		{
			AddObjectIndex(Object, CurrentObjectIndex++);
		}
		return ObjectToIndex[Object];
	}

	void AddObjectIndex(CObject* Object, int Index)
	{
		ObjectToIndex[Object] = Index;
		IndexToObject[Index] = Object;
	}

	CObject* GetObjectFromIndex(int Index)
	{
		return IndexToObject[Index];
	}


	rapidjson::Document::AllocatorType& GetAllocator() const { return JsonAllocator; }

protected:
	int CurrentObjectIndex;
	Map<CObject*, int> ObjectToIndex;
	Map<int, CObject*> IndexToObject;
	rapidjson::Document::AllocatorType& JsonAllocator;
};

/**
 * Base class for ?
 */
class CObject
{
	DECLARE_CLASS(CObject, CObject)

public:
	CObject(CObject* ThisOwner = nullptr);
	virtual ~CObject();

	/** Triggers when object is created during runtime */
	virtual void PreInit();

	/** Triggers when game started */
	virtual void Init();

	/** Object tick, called only in runtime */
	virtual void Tick(float DeltaTime);

	/**
	 * Serialize object into json
	 * 
	 * @param OutValue	json value that needs to be filled by the function
	 * @param Archive	Helper object to serialize pointers & recieve additional serialization info
	 * @return			true on success, false otherwise
	 */
	virtual bool Serialize(rapidjson::Value& OutValue, SArchive& Archive);

	/**
	 * Deserialize object from json into a valid object
	 *
	 * @param InValue	Json representation of this object
	 * @return			Valid CObject ptr on newly created object, nullptr otherwise
	 */
	virtual bool Deserialize(rapidjson::Value& InValue, SArchive& Archive);

	void AddComponent(CComponent* Component);

	/** Helper functions */
	const String& GetObjectName() const { return ObjectName; }
	Vector<CComponent*>& GetComponents();

	template <typename T>
	T* GetComponentByClass()
	{
		for (auto& Component : GetComponents())
		{
			if (T* FoundComponent = dynamic_cast<T*>(Component))
			{
				return FoundComponent;
			}
		}
		return nullptr;
	}

#ifdef WITH_EDITOR
	Vector<SField>& GetEditorFields()
	{
		if (!bFilledEditorFields)
		{
			FillEditorFields();
			bFilledEditorFields = true;
		}
		return EditorFields; 
	};
	virtual void FillEditorFields();

	void AddEditorField(const char* FieldName, void* FieldAdress, EFieldType FieldType);
	void AddEditorField(const SField& Field);

	/** Object tick, called only in runtime */
	virtual void EditorTick(float DeltaTime);

	/** Called when any editor property of this object was changed in editor */
	virtual void PostEditChangeProperty(SField& ChangedValue);

	Vector<SField> EditorFields;
	bool bFilledEditorFields = false;
#endif

	/** Whether this object is active and ticking */
	bool bTicking;

	bool bMustBeSerialized;

protected:
	/** Owner of this object */
	CObject* Owner;

	String ObjectName;

	Vector<CComponent*> Components;
};


// Create new object in runtime/editor
template <typename T>
CObject* NewObject(CObject* Owner)
{
	CObjectManager* ObjectManager = GEngine->GetObjectManager();

	CObject* CreatedObject = new T(Owner);

	bool bIsObject = false;
	if constexpr (std::is_base_of<CComponent, T>::value)
	{
		if constexpr (std::is_base_of<CRenderComponent, T>::value)
		{
			ObjectManager->AddRenderComponent(static_cast<CRenderComponent*>(CreatedObject));
		}

		if (Owner)
		{
			Owner->AddComponent(static_cast<CComponent*>(CreatedObject));
		}
	}
	else
	{
		bIsObject = true;
	}

	CreatedObject->PreInit();

	if (bIsObject)
	{
		ObjectManager->AddObject(CreatedObject);
	}

	return CreatedObject;
}

// Helper function to avoid type casting when using NewObject
template <typename T>
T* CreateObject(CObject* Owner)
{
	return static_cast<T*>(NewObject<T>(Owner));
}