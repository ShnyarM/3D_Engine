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

class BlendNormalVertex4
{
public:
	BlendNormalVertex4() = default;
	BlendNormalVertex4(const Vec4& pos, const Vec3& normal)
		:
		pos(pos),
		n(normal)
	{
	}
	BlendNormalVertex4(const Vec3& pos, const BlendNormalVertex4& v)
		:
		pos(pos),
		n(v.n)
	{
	}
	BlendNormalVertex4& operator=(const BlendNormalVertex4& rhs)
	{
		pos = rhs.pos;
		n = rhs.n;
		return *this;
	}
	BlendNormalVertex4& operator+=(const BlendNormalVertex4& rhs)
	{
		pos += rhs.pos;
		n += rhs.n;
		return *this;
	}
	BlendNormalVertex4& operator-=(const BlendNormalVertex4& rhs)
	{
		pos -= rhs.pos;
		n -= rhs.n;
		return *this;
	}
	BlendNormalVertex4 operator+(const BlendNormalVertex4& rhs) const
	{
		return BlendNormalVertex4(*this) += rhs;
	}
	BlendNormalVertex4 operator-(const BlendNormalVertex4& rhs) const
	{
		return BlendNormalVertex4(*this) -= rhs;
	}
	BlendNormalVertex4& operator*=(const float rhs)
	{
		pos *= rhs;
		n *= rhs;
		return *this;
	}
	BlendNormalVertex4 operator*(const float rhs) const
	{
		return BlendNormalVertex4(*this) *= rhs;
	}
	BlendNormalVertex4& operator/=(const float rhs)
	{
		pos /= rhs;
		n /= rhs;
		return *this;
	}
	BlendNormalVertex4 operator/(const float rhs) const
	{
		return BlendNormalVertex4(*this) /= rhs;
	}

	Vec4 pos;
	Vec3 n;
};