#pragma once

#include "Core/Public/Object.h"

class CActor;

class CComponent : public CObject
{
	DECLARE_CLASS(CComponent, CObject)

public:
	CComponent(CObject* Owner);
	~CComponent() {}

	/** Triggers when object is created during runtime */
	virtual void PreInit() override;

	/** Triggers when game started */
	virtual void Init() override;

	/** Object tick, called only in runtime */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Serialize object into json
	 *
	 * @param OutValue	json value that needs to be filled by the function
	 * @param Archive	Helper object to serialize pointers & recieve additional serialization info
	 * @return			true on success, false otherwise
	 */
	virtual bool Serialize(rapidjson::Value& OutValue, SArchive& Archive) override;

#ifdef WITH_EDITOR
	virtual void EditorTick(float DeltaTime) override;
#endif

protected:
	CActor* ActorOwner;
};