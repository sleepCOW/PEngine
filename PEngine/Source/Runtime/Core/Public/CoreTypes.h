#pragma once

#include <string>

struct SWindowParam
{
	std::string WindowTitle;
	int Width = 0;
	int Height = 0;
	bool bFullscreen = false;
};