#pragma once
#include "Vec3.h"

class NormalVertex
{
public:
	NormalVertex() = default;
	NormalVertex(const Vec3& pos, const Vec3& normal)
		:
		pos(pos),
		n(normal)
	{
	}
	NormalVertex(const Vec3& pos, const NormalVertex& v)
		:
		pos(pos),
		n(v.n)
	{
	}
	NormalVertex& operator=(const NormalVertex& rhs)
	{
		pos = rhs.pos;
		n = rhs.n;
		return *this;
	}
	NormalVertex& operator+=(const NormalVertex& rhs)
	{
		pos += rhs.pos;
		return *this;
	}
	NormalVertex& operator-=(const NormalVertex& rhs)
	{
		pos -= rhs.pos;
		return *this;
	}
	NormalVertex operator+(const NormalVertex& rhs) const
	{
		return NormalVertex(*this) += rhs;
	}
	NormalVertex operator-(const NormalVertex& rhs) const
	{
		return NormalVertex(*this) -= rhs;
	}
	NormalVertex& operator*=(const float rhs)
	{
		pos *= rhs;
		return *this;
	}
	NormalVertex operator*(const float rhs) const
	{
		return NormalVertex(*this) *= rhs;
	}
	NormalVertex& operator/=(const float rhs)
	{
		pos /= rhs;
		return *this;
	}
	NormalVertex operator/(const float rhs) const
	{
		return NormalVertex(*this) /= rhs;
	}

	Vec3 pos;
	Vec3 n;
};