#pragma once
#include "Vec3.h"

class TextureVertex
{
public:
	TextureVertex() = default;
	TextureVertex(const Vec3& pos, const Vec2& tPos)
		:
		pos(pos),
		t(tPos)
	{
	}
	TextureVertex(const Vec3& pos, const TextureVertex& v)
		:
		pos(pos),
		t(v.t)
	{
	}
	TextureVertex& operator=(const TextureVertex& rhs)
	{
		pos = rhs.pos;
		t = rhs.t;
		return *this;
	}
	TextureVertex& operator+=(const TextureVertex& rhs)
	{
		pos += rhs.pos;
		t += rhs.t;
		return *this;
	}
	TextureVertex& operator-=(const TextureVertex& rhs)
	{
		pos -= rhs.pos;
		t -= rhs.t;
		return *this;
	}
	TextureVertex operator+(const TextureVertex& rhs) const
	{
		return TextureVertex(*this) += rhs;
	}
	TextureVertex operator-(const TextureVertex& rhs) const
	{
		return TextureVertex(*this) -= rhs;
	}
	TextureVertex& operator*=(const float rhs)
	{
		pos *= rhs;
		t *= rhs;
		return *this;
	}
	TextureVertex operator*(const float rhs) const
	{
		return TextureVertex(*this) *= rhs;
	}
	TextureVertex& operator/=(const float rhs)
	{
		pos /= rhs;
		t /= rhs;
		return *this;
	}
	TextureVertex operator/(const float rhs) const
	{
		return TextureVertex(*this) /= rhs;
	}

	Vec3 pos;
	Vec2 t;
};