#pragma once
#include "Vec3.h"

class DefaultVertex
{
public:
	DefaultVertex() = default;
	DefaultVertex(const Vec3& pos )
		:
		pos(pos)
	{
	}
	DefaultVertex(const Vec3& pos, const DefaultVertex& v)
		:
		pos(pos)
	{
	}
	DefaultVertex& operator=(const DefaultVertex& rhs)
	{
		pos = rhs.pos;
		return *this;
	}
	DefaultVertex& operator+=(const DefaultVertex& rhs)
	{
		pos += rhs.pos;
		return *this;
	}
	DefaultVertex& operator-=(const DefaultVertex& rhs)
	{
		pos -= rhs.pos;
		return *this;
	}
	DefaultVertex operator+(const DefaultVertex& rhs) const
	{
		return DefaultVertex(*this) += rhs;
	}
	DefaultVertex operator-(const DefaultVertex& rhs) const
	{
		return DefaultVertex(*this) -= rhs;
	}
	DefaultVertex& operator*=(const float rhs)
	{
		pos *= rhs;
		return *this;
	}
	DefaultVertex operator*(const float rhs) const
	{
		return DefaultVertex(*this) *= rhs;
	}
	DefaultVertex& operator/=(const float rhs)
	{
		pos /= rhs;
		return *this;
	}
	DefaultVertex operator/(const float rhs) const
	{
		return DefaultVertex(*this) /= rhs;
	}

	Vec3 pos;
};
