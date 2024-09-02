#pragma once
#include "Vec3.h"

class BlendNormalVertex
{
public:
	BlendNormalVertex() = default;
	BlendNormalVertex(const Vec3& pos, const Vec3& normal)
		:
		pos(pos),
		n(normal)
	{
	}
	BlendNormalVertex(const Vec3& pos, const BlendNormalVertex& v)
		:
		pos(pos),
		n(v.n)
	{
	}
	BlendNormalVertex& operator=(const BlendNormalVertex& rhs)
	{
		pos = rhs.pos;
		n = rhs.n;
		return *this;
	}
	BlendNormalVertex& operator+=(const BlendNormalVertex& rhs)
	{
		pos += rhs.pos;
		n += rhs.n;
		return *this;
	}
	BlendNormalVertex& operator-=(const BlendNormalVertex& rhs)
	{
		pos -= rhs.pos;
		n -= rhs.n;
		return *this;
	}
	BlendNormalVertex operator+(const BlendNormalVertex& rhs) const
	{
		return BlendNormalVertex(*this) += rhs;
	}
	BlendNormalVertex operator-(const BlendNormalVertex& rhs) const
	{
		return BlendNormalVertex(*this) -= rhs;
	}
	BlendNormalVertex& operator*=(const float rhs)
	{
		pos *= rhs;
		n *= rhs;
		return *this;
	}
	BlendNormalVertex operator*(const float rhs) const
	{
		return BlendNormalVertex(*this) *= rhs;
	}
	BlendNormalVertex& operator/=(const float rhs)
	{
		pos /= rhs;
		n /= rhs;
		return *this;
	}
	BlendNormalVertex operator/(const float rhs) const
	{
		return BlendNormalVertex(*this) /= rhs;
	}

	Vec3 pos;
	Vec3 n;
};