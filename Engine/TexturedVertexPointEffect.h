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
	float linearAttenuation = 0.3f;
	float quadraticAttenuation = 0.75f;
	float constantAttenuation = 0.4f;
	float range = 3.0f;
};

class TexturedVertexPointEffect
{
public:
	// define input vertex
	typedef TextureNormalVertex Vertex;

	class VertexShader : public BaseVertexShader, public BasePointShader<PointDiffusee>
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

	public:
		// Binds camera view to both base vertex shader and point light shader
		void BindCompleteView(const Mat4& newView)
		{
			BindView(newView);
			SetCamView(newView);
		}

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
			Vec3 light = in.light.GetSaturated();
			pixelColor.x *= light.x;
			pixelColor.y *= light.y;
			pixelColor.z *= light.z;
			return Color(pixelColor);
		}
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};