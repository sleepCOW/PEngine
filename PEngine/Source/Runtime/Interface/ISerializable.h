#pragma once

#include "rapidJSON/document.h"

class ISerializable
{
public:

	rapidjson::Value Serialize(bool& bSuccess) = 0;
	rapidjson::Value Deserialize(bool& bSuccess) = 0;
};