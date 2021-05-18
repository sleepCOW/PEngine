#pragma once

#include "CoreMinimal.h"
#include "Core/Public/Object.h"

/**
 * Class that has location on scene
 */
class CActor : public CObject
{
	DECLARE_CLASS(CActor, CObject)

public:
	CActor(CObject* ThisOwner = nullptr);

	/** Triggers when object is created during runtime */
	virtual void PreInit();

	virtual void Tick(float DeltaTime) override;

	virtual bool Serialize(rapidjson::Value& OutValue, SArchive& Archive) override;
	virtual bool Deserialize(rapidjson::Value& InValue, SArchive& Archive) override;

#ifdef WITH_EDITOR
	virtual void FillEditorFields() override;
#endif

	/** Helper functions */
	SVector GetLocation() const { return Location; }
	void SetLocation(const SVector& Value) { Location = Value; }

protected:
	SVector Location;
};