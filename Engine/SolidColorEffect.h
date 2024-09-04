#pragma once
#include "Vec3.h"
#include "Vec2.h"
#include "Colors.h"
#include <memory>
#include "Surface.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"
#include "SolidColorVertex.h"

class SolidColorEffect
{
public:
	typedef SolidColorVertex Vertex;

	//Define VertexShader
	class VertexShader
	{
	public:
		// normal blendNormalVertex, but add original worldposition as additional information for pixelshader
		class Output
		{
		public:
			Output() = default;
			Output(const Vec4& pos, Color color)
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

			Vec4 pos;
			Color color;
		};

	public:
		void BindWorldTransformation(const Mat4& transform)
		{
			worldTransform = transform;
		}
		void BindProjection(const Mat4& projection)
		{
			worldProjection = projection;
		}
		const Mat4 GetProj() const
		{
			return worldProjection;
		}
		Output operator()(const Vertex& input)
		{
			Vec4 newPos = Vec4(input.pos) * worldTransform;
			return { Vec4(input.pos)*worldTransform*worldProjection, input.color };
		}

	private:
		Mat4 worldTransform = Mat4::Identity();
		Mat4 worldProjection = Mat4::Identity();
	};

	//Define GeometryShader
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

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
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};