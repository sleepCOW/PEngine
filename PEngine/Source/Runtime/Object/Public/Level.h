#pragma once

#include "Core/Public/CoreMinimal.h"
#include "Core/Public/Object.h"

class CLevel : public CObject
{
	DECLARE_CLASS(CLevel, CObject)

public:


protected:
	rapidjson::Document LevelJSON;
};