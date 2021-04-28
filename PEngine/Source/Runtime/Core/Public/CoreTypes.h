#pragma once

#include <string>

#ifdef UNICODE
using TChar = wchar_t;
#else
using TChar = char;
#endif

using String = std::string;

constexpr int INDEX_NONE = -1;

struct SWindowParam
{
	String WindowTitle;
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
	PTR,
	TEXTURE,
	RECTANGLE
};

struct SField
{
	void* PField;
	const char* FieldName;
	EFieldType FieldType;

	/** Get data converted to desired type */
	template <typename T>
	T& Get()
	{
		return *(reinterpret_cast<T*>(PField));
	}
};
#endif WITH_EDITOR