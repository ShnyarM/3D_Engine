#pragma once
#include "Vec3.h"
#include "Vec2.h"

struct TexVertex
{
	TexVertex(const Vec3& pos, const Vec2& tPos) 
		:
		pos(pos),
		tPos(tPos)
	{}
	TexVertex& operator=(const TexVertex& rhs)
	{
		pos = rhs.pos;
		tPos = rhs.tPos;
		return *this;
	}
	TexVertex& operator+=(const TexVertex& rhs)
	{
		pos += rhs.pos;
		tPos += rhs.tPos;
		return *this;
	}
	TexVertex& operator-=(const TexVertex& rhs)
	{
		pos -= rhs.pos;
		tPos -= rhs.tPos;
		return *this;
	}
	TexVertex operator+(const TexVertex& rhs) const
	{
		return TexVertex(*this) += rhs;
	}
	TexVertex operator-(const TexVertex& rhs) const
	{
		return TexVertex(*this) -= rhs;
	}
	TexVertex& operator*=(const float rhs)
	{
		pos *= rhs;
		tPos *= rhs;
		return *this;
	}
	TexVertex operator*(const float rhs) const
	{
		return TexVertex(*this) *= rhs;
	}
	TexVertex& operator/=(const float rhs)
	{
		pos /= rhs;
		tPos /= rhs;
		return *this;
	}
	TexVertex operator/(const float rhs) const
	{
		return TexVertex(*this) /= rhs;
	}
	TexVertex InterpolateTo(const TexVertex& rhs, const float alpha) const
	{
		return { pos.InterpolateTo(rhs.pos, alpha), tPos.InterpolateTo(rhs.tPos, alpha) };
	}

	Vec3 pos;
	Vec2 tPos;
};