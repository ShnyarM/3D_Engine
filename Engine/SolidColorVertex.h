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

class SolidColorVertex4
{
public:
	SolidColorVertex4() = default;
	SolidColorVertex4(const Vec4& pos, Color color)
		:
		pos(pos),
		color(color)
	{
	}
	SolidColorVertex4(const Vec3& pos, const SolidColorVertex4& v)
		:
		pos(pos),
		color(v.color)
	{
	}
	SolidColorVertex4& operator=(const SolidColorVertex4& rhs)
	{
		pos = rhs.pos;
		color = rhs.color;
		return *this;
	}
	SolidColorVertex4& operator+=(const SolidColorVertex4& rhs)
	{
		pos += rhs.pos;
		return *this;
	}
	SolidColorVertex4& operator-=(const SolidColorVertex4& rhs)
	{
		pos -= rhs.pos;
		return *this;
	}
	SolidColorVertex4 operator+(const SolidColorVertex4& rhs) const
	{
		return SolidColorVertex4(*this) += rhs;
	}
	SolidColorVertex4 operator-(const SolidColorVertex4& rhs) const
	{
		return SolidColorVertex4(*this) -= rhs;
	}
	SolidColorVertex4& operator*=(const float rhs)
	{
		pos *= rhs;
		return *this;
	}
	SolidColorVertex4 operator*(const float rhs) const
	{
		return SolidColorVertex4(*this) *= rhs;
	}
	SolidColorVertex4& operator/=(const float rhs)
	{
		pos /= rhs;
		return *this;
	}
	SolidColorVertex4 operator/(const float rhs) const
	{
		return SolidColorVertex4(*this) /= rhs;
	}

	Vec4 pos;
	Color color;
};