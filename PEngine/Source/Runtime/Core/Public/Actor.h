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

	virtual void Tick(float DeltaTime) override;

#ifdef WITH_EDITOR
	virtual void FillEditorFields() override;
#endif

protected:

	SVector Location;
};