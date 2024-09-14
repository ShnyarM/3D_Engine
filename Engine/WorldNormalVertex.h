#pragma once
#include "Vec4.h"

// Vertex which has blending normal and worldpos attribute
class WorldNormalVertex
{
public:
	WorldNormalVertex() = default;
	WorldNormalVertex(const Vec3& pos, const Vec3& normal, const Vec3& worldPos)
		:
		pos(pos),
		n(normal),
		worldPos(worldPos)
	{
	}
	WorldNormalVertex(const Vec3& pos, const WorldNormalVertex& v)
		:
		pos(pos),
		n(v.n),
		worldPos(v.worldPos)
	{
	}
	WorldNormalVertex& operator=(const WorldNormalVertex& rhs)
	{
		pos = rhs.pos;
		n = rhs.n;
		worldPos = rhs.worldPos;
		return *this;
	}
	WorldNormalVertex& operator+=(const WorldNormalVertex& rhs)
	{
		pos += rhs.pos;
		n += rhs.n;
		worldPos += rhs.worldPos;
		return *this;
	}
	WorldNormalVertex& operator-=(const WorldNormalVertex& rhs)
	{
		pos -= rhs.pos;
		n -= rhs.n;
		worldPos -= rhs.worldPos;
		return *this;
	}
	WorldNormalVertex operator+(const WorldNormalVertex& rhs) const
	{
		return WorldNormalVertex(*this) += rhs;
	}
	WorldNormalVertex operator-(const WorldNormalVertex& rhs) const
	{
		return WorldNormalVertex(*this) -= rhs;
	}
	WorldNormalVertex& operator*=(const float rhs)
	{
		pos *= rhs;
		n *= rhs;
		worldPos *= rhs;
		return *this;
	}
	WorldNormalVertex operator*(const float rhs) const
	{
		return WorldNormalVertex(*this) *= rhs;
	}
	WorldNormalVertex& operator/=(const float rhs)
	{
		pos /= rhs;
		n /= rhs;
		worldPos /= rhs;
		return *this;
	}
	WorldNormalVertex operator/(const float rhs) const
	{
		return WorldNormalVertex(*this) /= rhs;
	}

	Vec3 pos;
	Vec3 n;
	Vec3 worldPos;
};

// Vertex with blending normal and worldPos attribute
// uses Vec4
class WorldNormalVertex4
{
public:
	WorldNormalVertex4() = default;
	WorldNormalVertex4(const Vec4& pos, const Vec3& normal, const Vec3& worldPos)
		:
		pos(pos),
		n(normal),
		worldPos(worldPos)
	{
	}
	WorldNormalVertex4(const Vec3& pos, const WorldNormalVertex4& v)
		:
		pos(pos),
		n(v.n),
		worldPos(v.worldPos)
	{
	}
	WorldNormalVertex4& operator=(const WorldNormalVertex4& rhs)
	{
		pos = rhs.pos;
		n = rhs.n;
		worldPos = rhs.worldPos;
		return *this;
	}
	WorldNormalVertex4& operator+=(const WorldNormalVertex4& rhs)
	{
		pos += rhs.pos;
		n += rhs.n;
		worldPos += rhs.worldPos;
		return *this;
	}
	WorldNormalVertex4& operator-=(const WorldNormalVertex4& rhs)
	{
		pos -= rhs.pos;
		n -= rhs.n;
		worldPos -= rhs.worldPos;
		return *this;
	}
	WorldNormalVertex4 operator+(const WorldNormalVertex4& rhs) const
	{
		return WorldNormalVertex4(*this) += rhs;
	}
	WorldNormalVertex4 operator-(const WorldNormalVertex4& rhs) const
	{
		return WorldNormalVertex4(*this) -= rhs;
	}
	WorldNormalVertex4& operator*=(const float rhs)
	{
		pos *= rhs;
		n *= rhs;
		worldPos *= rhs;
		return *this;
	}
	WorldNormalVertex4 operator*(const float rhs) const
	{
		return WorldNormalVertex4(*this) *= rhs;
	}
	WorldNormalVertex4& operator/=(const float rhs)
	{
		pos /= rhs;
		n /= rhs;
		worldPos /= rhs;
		return *this;
	}
	WorldNormalVertex4 operator/(const float rhs) const
	{
		return WorldNormalVertex4(*this) /= rhs;
	}

	Vec4 pos;
	Vec3 n;
	Vec3 worldPos;
};