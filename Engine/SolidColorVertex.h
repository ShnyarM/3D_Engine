#pragma once
#include "Vec3.h"
#include "Colors.h"

class SolidColorVertex
{
public:
	SolidColorVertex() = default;
	SolidColorVertex(const Vec3& pos, const Color& color)
		:
		pos(pos),
		color(color)
	{
	}
	SolidColorVertex(const Vec3& pos, const SolidColorVertex& v)
		:
		pos(pos),
		color(v.color)
	{
	}
	SolidColorVertex& operator=(const SolidColorVertex& rhs)
	{
		pos = rhs.pos;
		color = rhs.color;
		return *this;
	}
	SolidColorVertex& operator+=(const SolidColorVertex& rhs)
	{
		pos += rhs.pos;
		return *this;
	}
	SolidColorVertex& operator-=(const SolidColorVertex& rhs)
	{
		pos -= rhs.pos;
		return *this;
	}
	SolidColorVertex operator+(const SolidColorVertex& rhs) const
	{
		return SolidColorVertex(*this) += rhs;
	}
	SolidColorVertex operator-(const SolidColorVertex& rhs) const
	{
		return SolidColorVertex(*this) -= rhs;
	}
	SolidColorVertex& operator*=(const float rhs)
	{
		pos *= rhs;
		return *this;
	}
	SolidColorVertex operator*(const float rhs) const
	{
		return SolidColorVertex(*this) *= rhs;
	}
	SolidColorVertex& operator/=(const float rhs)
	{
		pos /= rhs;
		return *this;
	}
	SolidColorVertex operator/(const float rhs) const
	{
		return SolidColorVertex(*this) /= rhs;
	}

	Vec3 pos;
	Color color;
};