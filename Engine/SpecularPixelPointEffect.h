#pragma once
#include "BlendNormalVertex.h"
#include "DefaultGeometryShader.h"
#include "Colors.h"
#include "Triangle.h"
#include <utility>
#include <cmath>
#include "BlendColorVertex.h"
#include "Mat.h"
#include "BaseVertexShader.h"
#include "BaseSpecularShader.h"

class SpecularPixelPointEffect
{
public:
	// Define Input Vertex
	typedef BlendNormalVertex Vertex;

	// Define VertexShader, default vertexShader but rotates normal
	class VertexShader : public BaseVertexShader
	{
	public:
		// normal blendNormalVertex, but add original worldposition as additional information for pixelshader
		class Output
		{
		public:
			Output() = default;
			Output(const Vec4& pos, const Vec3& normal, const Vec3& worldPos)
				:
				pos(pos),
				n(normal),
				worldPos(worldPos)
			{
			}
			Output(const Vec3& pos, const Output& v)
				:
				pos(pos),
				n(v.n),
				worldPos(v.worldPos)
			{
			}
			Output& operator=(const Output& rhs)
			{
				pos = rhs.pos;
				n = rhs.n;
				worldPos = rhs.worldPos;
				return *this;
			}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				n += rhs.n;
				worldPos += rhs.worldPos;
				return *this;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				n -= rhs.n;
				worldPos -= rhs.worldPos;
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
				n *= rhs;
				worldPos *= rhs;
				return *this;
			}
			Output operator*(const float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(const float rhs)
			{
				pos /= rhs;
				n /= rhs;
				worldPos /= rhs;
				return *this;
			}
			Output operator/(const float rhs) const
			{
				return Output(*this) /= rhs;
			}

			Vec4 pos;
			Vec3 n;
			Vec3 worldPos;
		};
	public:
		Output operator()(const Vertex& input)
		{
			return { Vec4(input.pos) * entireTransform, Vec4(input.n, 0.0f) * entireWorldTransform, Vec4(input.pos) * entireWorldTransform };
		}
	};

public:
	// Default GeometryShader
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

public:
	class PixelShader : public BaseSpecularShader<>
	{
	public:
		Color operator()(const VertexShader::Output& input)
		{
			return this->Shade(input);
		}
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};
