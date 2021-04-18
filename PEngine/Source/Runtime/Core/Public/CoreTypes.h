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

#ifdef WITH_EDITOR
enum class EFieldType
{
	NONE,
	INT,
	FLOAT,
	STRING,
	MATH_VECTOR,
	PTR
};

struct SField
{
	void* PField;
	EFieldType FieldType;
};
#endif WITH_EDITOR
