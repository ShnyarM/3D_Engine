#pragma once
#include "Surface.h"

template<typename T>
class _Rect
{
public:
	_Rect(T x, T y, T width, T height)
		:
		x(x),
		y(y),
		width(width),
		height(height)
	{}

	_Rect(const Surface& sprite)
		:
		x((T)0),
		y((T)0),
		width((T)sprite.GetWidth()),
		height((T)sprite.GetHeight())
	{}

public:
	T x;
	T y;
	T width;
	T height;
};

typedef _Rect<int> RectI;