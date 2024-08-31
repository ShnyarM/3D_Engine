#pragma once
#include "Vec3.h"

class TextureNormalVertex
{
public:
	TextureNormalVertex() = default;
	TextureNormalVertex(const Vec3& pos, const Vec2& tPos, const Vec3& normal)
		:
		pos(pos),
		t(tPos),
		n(normal)
	{
	}
	TextureNormalVertex(const Vec3& pos, const TextureNormalVertex& v)
		:
		pos(pos),
		t(v.t),
		n(v.n)
	{
	}
	TextureNormalVertex& operator=(const TextureNormalVertex& rhs)
	{
		pos = rhs.pos;
		t = rhs.t;
		n = rhs.n;
		return *this;
	}
	TextureNormalVertex& operator+=(const TextureNormalVertex& rhs)
	{
		pos += rhs.pos;
		t += rhs.t;
		return *this;
	}
	TextureNormalVertex& operator-=(const TextureNormalVertex& rhs)
	{
		pos -= rhs.pos;
		t -= rhs.t;
		return *this;
	}
	TextureNormalVertex operator+(const TextureNormalVertex& rhs) const
	{
		return TextureNormalVertex(*this) += rhs;
	}
	TextureNormalVertex operator-(const TextureNormalVertex& rhs) const
	{
		return TextureNormalVertex(*this) -= rhs;
	}
	TextureNormalVertex& operator*=(const float rhs)
	{
		pos *= rhs;
		t *= rhs;
		return *this;
	}
	TextureNormalVertex operator*(const float rhs) const
	{
		return TextureNormalVertex(*this) *= rhs;
	}
	TextureNormalVertex& operator/=(const float rhs)
	{
		pos /= rhs;
		t /= rhs;
		return *this;
	}
	TextureNormalVertex operator/(const float rhs) const
	{
		return TextureNormalVertex(*this) /= rhs;
	}

	Vec3 pos;
	Vec2 t;
	Vec3 n;
};