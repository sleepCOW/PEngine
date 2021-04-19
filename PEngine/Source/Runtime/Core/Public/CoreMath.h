#pragma once

#include "CoreTypes.h"
#include <cmath>

struct SVector
{
	SVector(float x = 0.f, float y = 0.f) : X(x), Y(y)
	{}

	SVector Add(const SVector& Other)
	{
		return { X + Other.X, Y + Other.Y };
	}
	
	SVector Add(float InX, float InY)
	{
		return { X + InX, Y + InY };
	}

	SVector Substract(const SVector& Other)
	{
		return { X - Other.X, Y - Other.Y };
	}

	float DotProduct(const SVector& Other)
	{
		return X * Other.X + Y * Other.Y;
	}

	void Normalize()
	{
		X /= Length();
		Y /= Length();
	}

	float Length()
	{
		return sqrt(X * X + Y * Y);
	}

	void operator +=(const SVector& Other) { *this = Add(Other); }
	void operator -=(const SVector& Other) { *this = Substract(Other); }

	static SVector ZeroVector() { return {0, 0}; }

	float X;
	float Y;
};