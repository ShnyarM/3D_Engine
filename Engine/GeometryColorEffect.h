#pragma once
#include "DefaultVertex.h"
#include "DefaultVertexShader.h"
#include "Colors.h"
#include "Triangle.h"
#include <vector>


class GeometryColorEffect
{
public:
	// Define Input Vertex
	typedef DefaultVertex Vertex;

	// Define VertexShader
	typedef DefaultVertexShader<Vertex> VertexShader;

public:
	class GeometryShader
	{
	public:
		class Output
		{
		public:
			Output() = default;
			Output(const Vec3& pos, const Color& color)
				:
				pos(pos),
				color(color)
			{
			}
			Output(const Vec3& pos, const Output& v)
				:
				pos(pos),
				color(v.color)
			{
			}
			Output& operator=(const Output& rhs)
			{
				pos = rhs.pos;
				color = rhs.color;
				return *this;
			}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				return *this;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator*=(const float rhs)
			{
				pos *= rhs;
				return *this;
			}
			Output operator*(const float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(const float rhs)
			{
				pos /= rhs;
				return *this;
			}
			Output operator/(const float rhs) const
			{
				return Output(*this) /= rhs;
			}

			Vec3 pos;
			Color color;
		};

		void BindColors(std::vector<Color> incoming)
		{
			colors = incoming;
		}

		Triangle<Output> operator()(const Vertex& v0, const Vertex& v1, const Vertex& v2, const size_t index)
		{
			Color& colorToUse = colors[index / 2];
			return { {v0.pos, colorToUse}, {v1.pos, colorToUse}, {v2.pos, colorToUse} };
		}

	private:
		std::vector<Color> colors;
	};

	class PixelShader
	{
		typedef GeometryShader::Output Input;

	public:
		Color operator()(const Input& in)
		{
			return in.color;
		}
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};