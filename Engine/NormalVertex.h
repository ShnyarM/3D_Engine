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

class NormalVertex4
{
public:
	NormalVertex4() = default;
	NormalVertex4(const Vec4& pos, const Vec3& normal)
		:
		pos(pos),
		n(normal)
	{
	}
	NormalVertex4(const Vec3& pos, const NormalVertex4& v)
		:
		pos(pos),
		n(v.n)
	{
	}
	NormalVertex4& operator=(const NormalVertex4& rhs)
	{
		pos = rhs.pos;
		n = rhs.n;
		return *this;
	}
	NormalVertex4& operator+=(const NormalVertex4& rhs)
	{
		pos += rhs.pos;
		return *this;
	}
	NormalVertex4& operator-=(const NormalVertex4& rhs)
	{
		pos -= rhs.pos;
		return *this;
	}
	NormalVertex4 operator+(const NormalVertex4& rhs) const
	{
		return NormalVertex4(*this) += rhs;
	}
	NormalVertex4 operator-(const NormalVertex4& rhs) const
	{
		return NormalVertex4(*this) -= rhs;
	}
	NormalVertex4& operator*=(const float rhs)
	{
		pos *= rhs;
		return *this;
	}
	NormalVertex4 operator*(const float rhs) const
	{
		return NormalVertex4(*this) *= rhs;
	}
	NormalVertex4& operator/=(const float rhs)
	{
		pos /= rhs;
		return *this;
	}
	NormalVertex4 operator/(const float rhs) const
	{
		return NormalVertex4(*this) /= rhs;
	}

	Vec4 pos;
	Vec3 n;
};