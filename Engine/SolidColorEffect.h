#pragma once
#include "Vec3.h"
#include "Vec2.h"
#include "Colors.h"
#include <memory>
#include "Surface.h"

class SolidColorEffect
{
public:
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec3& pos, const Color& color)
			:
			pos(pos),
			color(color)
		{
		}
		Vertex(const Vec3& pos, const Vertex& v)
			:
			pos(pos),
			color(v.color)
		{
		}
		Vertex& operator=(const Vertex& rhs)
		{
			pos = rhs.pos;
			color = rhs.color;
			return *this;
		}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			return *this;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(const float rhs)
		{
			pos *= rhs;
			return *this;
		}
		Vertex operator*(const float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(const float rhs)
		{
			pos /= rhs;
			return *this;
		}
		Vertex operator/(const float rhs) const
		{
			return Vertex(*this) /= rhs;
		}

		Vec3 pos;
		Color color;
	};

	class PixelShader
	{
	public:
		// Input class is Vertex
		// Get Color based of vertex
		template <class Input>
		Color operator()(const Input& in)
		{
			return in.color;
		}
	};

public:
	PixelShader ps;
};