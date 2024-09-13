#pragma once
#include "Vec3.h"

class TextureBlendNormalVertex
{
public:
	TextureBlendNormalVertex() = default;
	TextureBlendNormalVertex(const Vec3& pos, const Vec2& t, const Vec3& normal)
		:
		pos(pos),
		t(t),
		n(normal)
	{
	}
	TextureBlendNormalVertex(const Vec3& pos, const TextureBlendNormalVertex& v)
		:
		pos(pos),
		t(v.t),
		n(v.n)
	{
	}
	TextureBlendNormalVertex& operator=(const TextureBlendNormalVertex& rhs)
	{
		pos = rhs.pos;
		t = rhs.t;
		n = rhs.n;
		return *this;
	}
	TextureBlendNormalVertex& operator+=(const TextureBlendNormalVertex& rhs)
	{
		pos += rhs.pos;
		t += rhs.t;
		n += rhs.n;
		return *this;
	}
	TextureBlendNormalVertex& operator-=(const TextureBlendNormalVertex& rhs)
	{
		pos -= rhs.pos;
		t -= rhs.t;
		n -= rhs.n;
		return *this;
	}
	TextureBlendNormalVertex operator+(const TextureBlendNormalVertex& rhs) const
	{
		return TextureBlendNormalVertex(*this) += rhs;
	}
	TextureBlendNormalVertex operator-(const TextureBlendNormalVertex& rhs) const
	{
		return TextureBlendNormalVertex(*this) -= rhs;
	}
	TextureBlendNormalVertex& operator*=(const float rhs)
	{
		pos *= rhs;
		t *= rhs;
		n *= rhs;
		return *this;
	}
	TextureBlendNormalVertex operator*(const float rhs) const
	{
		return TextureBlendNormalVertex(*this) *= rhs;
	}
	TextureBlendNormalVertex& operator/=(const float rhs)
	{
		pos /= rhs;
		t /= rhs;
		n /= rhs;
		return *this;
	}
	TextureBlendNormalVertex operator/(const float rhs) const
	{
		return TextureBlendNormalVertex(*this) /= rhs;
	}

	Vec3 pos;
	Vec2 t;
	Vec3 n;
};

class TextureBlendNormalVertexWorld4
{
public:
	TextureBlendNormalVertexWorld4() = default;
	TextureBlendNormalVertexWorld4(const Vec4& pos, const Vec2& t, const Vec3& normal, const Vec3& worldPos)
		:
		pos(pos),
		t(t),
		n(normal),
		worldPos(worldPos)
	{
	}
	TextureBlendNormalVertexWorld4(const Vec3& pos, const TextureBlendNormalVertexWorld4& v)
		:
		pos(pos),
		t(v.t),
		n(v.n),
		worldPos(worldPos)
	{
	}
	TextureBlendNormalVertexWorld4& operator=(const TextureBlendNormalVertexWorld4& rhs)
	{
		pos = rhs.pos;
		t = rhs.t;
		n = rhs.n;
		worldPos = rhs.worldPos;
		return *this;
	}
	TextureBlendNormalVertexWorld4& operator+=(const TextureBlendNormalVertexWorld4& rhs)
	{
		pos += rhs.pos;
		t += rhs.t;
		n += rhs.n;
		worldPos += rhs.worldPos;
		return *this;
	}
	TextureBlendNormalVertexWorld4& operator-=(const TextureBlendNormalVertexWorld4& rhs)
	{
		pos -= rhs.pos;
		t -= rhs.t;
		n -= rhs.n;
		worldPos -= rhs.worldPos;
		return *this;
	}
	TextureBlendNormalVertexWorld4 operator+(const TextureBlendNormalVertexWorld4& rhs) const
	{
		return TextureBlendNormalVertexWorld4(*this) += rhs;
	}
	TextureBlendNormalVertexWorld4 operator-(const TextureBlendNormalVertexWorld4& rhs) const
	{
		return TextureBlendNormalVertexWorld4(*this) -= rhs;
	}
	TextureBlendNormalVertexWorld4& operator*=(const float rhs)
	{
		pos *= rhs;
		t *= rhs;
		n *= rhs;
		worldPos *= rhs;
		return *this;
	}
	TextureBlendNormalVertexWorld4 operator*(const float rhs) const
	{
		return TextureBlendNormalVertexWorld4(*this) *= rhs;
	}
	TextureBlendNormalVertexWorld4& operator/=(const float rhs)
	{
		pos /= rhs;
		t /= rhs;
		n /= rhs;
		worldPos /= rhs;
		return *this;
	}
	TextureBlendNormalVertexWorld4 operator/(const float rhs) const
	{
		return TextureBlendNormalVertexWorld4(*this) /= rhs;
	}

	Vec4 pos;
	Vec2 t;
	Vec3 n;
	Vec3 worldPos;
};
