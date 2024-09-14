#pragma once
#include "TextureNormalVertex.h"
#include "BlendColorVertex.h"
#include "BaseVertexShader.h"
#include "BasePointShader.h"
#include "DefaultGeometryShader.h"
#include <memory>
#include "Surface.h"

struct PointDiffusee
{
	float linearAttenuation = 1.0f;
	float quadraticAttenuation = 2.62f;
	float constantAttenuation = 10.0f;
	float strength = 20.0f;
};

class TexturedVertexPointEffect
{
public:
	// define input vertex
	typedef TextureNormalVertex Vertex;

	class VertexShader : public BaseVertexShader, public BasePointShader<PointDiffuse>
	{
	public:
		class Output
		{
		public:
			Output() = default;
			Output(const Vec4& pos, const Vec2& tPos, const Vec3& light)
				:
				pos(pos),
				t(tPos),
				//n(normal),
				light(light)
			{
			}
			Output(const Vec3& pos, const Output& v)
				:
				pos(pos),
				t(v.t),
				//n(v.n),
				light(v.light)
			{
			}
			Output& operator=(const Output& rhs)
			{
				pos = rhs.pos;
				t = rhs.t;
				//n = rhs.n;
				light = rhs.light;
				return *this;
			}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				t += rhs.t;
				light += rhs.light;
				return *this;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				t -= rhs.t;
				light -= rhs.light;
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
				t *= rhs;
				light *= rhs;
				return *this;
			}
			Output operator*(const float rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(const float rhs)
			{
				pos /= rhs;
				t /= rhs;
				light /= rhs;
				return *this;
			}
			Output operator/(const float rhs) const
			{
				return Output(*this) /= rhs;
			}

			Vec4 pos;
			Vec2 t;
			//Vec3 n;
			Vec3 light;
		};

		Output operator()(const Vertex& input)
		{
			// apply worldtransform and camera view to be in same space as light
			WorldNormalVertex4 transformed = { Vec4(input.pos) * entireTransform, Vec4(input.n, 0.0f) * entireWorldTransform,  Vec4(input.pos) * entireWorldTransform};
			Vec3 light = LightLevel(transformed);

			return { transformed.pos, input.t, light };
		}
	};

	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

	class PixelShader : public BaseTextureShader
	{
	public:
		// Get Color based of vertex and current loaded texture
		Color operator()(const VertexShader::Output& in)
		{
			Vec3 pixelColor = Vec3(GetColor(in));
			pixelColor.x *= in.light.x;
			pixelColor.y *= in.light.y;
			pixelColor.z *= in.light.z;
			return Color(pixelColor);
		}
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};