#pragma once

#include <string>

#ifdef UNICODE
using TChar = wchar_t;
#else
using TChar = char;
#endif

using String = std::string;

struct SWindowParam
{
	std::string WindowTitle;
	int Width = 0;
	int Height = 0;
	bool bFullscreen = false;
};