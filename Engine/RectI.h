#pragma once
#include "Surface.h"

class RectI
{
public:
	RectI(int x, int y, int width, int height)
		:
		x(x),
		y(y),
		width(width),
		height(height)
	{}

	RectI(const Surface& sprite)
		:
		x(0),
		y(0),
		width(sprite.GetWidth()),
		height(sprite.GetHeight())
	{}

public:
	int x;
	int y;
	int width;
	int height;
};