#pragma once
#include "Vec3.h"

class BlendColorVertex
{
public:
	BlendColorVertex() = default;
	BlendColorVertex(const Vec3& pos, const Vec3& color)
		:
		pos(pos),
		color(color)
	{
	}
	BlendColorVertex(const Vec3& pos, const BlendColorVertex& v)
		:
		pos(pos),
		color(v.color)
	{
	}
	BlendColorVertex& operator=(const BlendColorVertex& rhs)
	{
		pos = rhs.pos;
		color = rhs.color;
		return *this;
	}
	BlendColorVertex& operator+=(const BlendColorVertex& rhs)
	{
		pos += rhs.pos;
		color += rhs.color;
		return *this;
	}
	BlendColorVertex& operator-=(const BlendColorVertex& rhs)
	{
		pos -= rhs.pos;
		color -= rhs.color;
		return *this;
	}
	BlendColorVertex operator+(const BlendColorVertex& rhs) const
	{
		return BlendColorVertex(*this) += rhs;
	}
	BlendColorVertex operator-(const BlendColorVertex& rhs) const
	{
		return BlendColorVertex(*this) -= rhs;
	}
	BlendColorVertex& operator*=(const float rhs)
	{
		pos *= rhs;
		color *= rhs;
		return *this;
	}
	BlendColorVertex operator*(const float rhs) const
	{
		return BlendColorVertex(*this) *= rhs;
	}
	BlendColorVertex& operator/=(const float rhs)
	{
		pos /= rhs;
		color /= rhs;
		return *this;
	}
	BlendColorVertex operator/(const float rhs) const
	{
		return BlendColorVertex(*this) /= rhs;
	}

	Vec3 pos;
	Vec3 color;
};

class BlendColorVertex4
{
public:
	BlendColorVertex4() = default;
	BlendColorVertex4(const Vec4& pos, const Vec3& color)
		:
		pos(pos),
		color(color)
	{
	}
	BlendColorVertex4(const Vec3& pos, const BlendColorVertex4& v)
		:
		pos(pos),
		color(v.color)
	{
	}
	BlendColorVertex4& operator=(const BlendColorVertex4& rhs)
	{
		pos = rhs.pos;
		color = rhs.color;
		return *this;
	}
	BlendColorVertex4& operator+=(const BlendColorVertex4& rhs)
	{
		pos += rhs.pos;
		color += rhs.color;
		return *this;
	}
	BlendColorVertex4& operator-=(const BlendColorVertex4& rhs)
	{
		pos -= rhs.pos;
		color -= rhs.color;
		return *this;
	}
	BlendColorVertex4 operator+(const BlendColorVertex4& rhs) const
	{
		return BlendColorVertex4(*this) += rhs;
	}
	BlendColorVertex4 operator-(const BlendColorVertex4& rhs) const
	{
		return BlendColorVertex4(*this) -= rhs;
	}
	BlendColorVertex4& operator*=(const float rhs)
	{
		pos *= rhs;
		color *= rhs;
		return *this;
	}
	BlendColorVertex4 operator*(const float rhs) const
	{
		return BlendColorVertex4(*this) *= rhs;
	}
	BlendColorVertex4& operator/=(const float rhs)
	{
		pos /= rhs;
		color /= rhs;
		return *this;
	}
	BlendColorVertex4 operator/(const float rhs) const
	{
		return BlendColorVertex4(*this) /= rhs;
	}

	Vec4 pos;
	Vec3 color;
};