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

	class VertexShader : public BaseVertexShader
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
		// light functions
		void SetSurfaceColor(const Color& newColor)
		{
			color = Vec3(newColor) / 255.0f;
		}
		void SetLightPos(const Vec3& newPos)
		{
			lightPos = newPos;
		}
		void SetLightColor(const Color& newColor)
		{
			lightColor = Vec3(newColor) / 255.0f;
		}
		void SetAmbient(const Vec3& newAmbient)
		{
			ambient = newAmbient;
		}

		Output operator()(const Vertex& input)
		{
			// Get information about position relative to light
			Vec3 newPos = Vec4(input.pos) * worldTransform;
			Vec3 diffVec = lightPos - newPos;
			float distance = diffVec.Len();
			Vec3 diffVecN = diffVec.GetNormalized();

			float attenuation = diffuseValues.strength / (diffuseValues.constantAttenuation + distance * diffuseValues.linearAttenuation + sq(distance) * diffuseValues.quadraticAttenuation); //distance factor, less if further away
			Vec3 strength = lightColor * attenuation * std::max(0.0f, (input.n * diffVecN)); //Strength which hits surface
			Vec3 light = color.GetHadamard(strength + ambient).Saturate(); // strength + ambient is light which hits surface in total
			return { Vec4(input.pos) * entireTransform, input.t, light };
		}

	private:
		Vec3 lightPos = { 0.0f, 0.0f, 0.5f }; //position of the light in the world
		Vec3 lightColor = { 1.0f, 1.0f, 1.0f }; //color of the light
		Vec3 ambient = { 0.05f, 0.05f, 0.05f }; //ambient light of the scene
		Vec3 color = { 1.0f, 1.0f, 1.0f }; //Color of the material

		PointDiffusee diffuseValues;
	};

	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

	class PixelShader
	{
	public:
		// Get Color based of vertex and current loaded texture
		Color operator()(const VertexShader::Output& in)
		{
			assert(textureLoaded);
			Vec3 pixelColor = Vec3(pTexture->GetPixel(
				(unsigned int)std::min(std::max(in.t.x * tWidth, 0.0f), tWidthMax),
				(unsigned int)std::min(std::max(in.t.y * tHeight, 0.0f), tHeightMax)));
			if (in.light.x > 0.5f)
			{
				int x = 1;
			}
			pixelColor.x *= in.light.x;
			pixelColor.y *= in.light.y;
			pixelColor.z *= in.light.z;
			return Color(pixelColor);
		}

		void BindTexture(const std::wstring& filename)
		{
			pTexture = std::make_unique<Surface>(Surface::FromFile(filename));
			tWidth = pTexture->GetWidth();
			tHeight = pTexture->GetHeight();
			tWidthMax = tWidth - 1.0f;
			tHeightMax = tHeight - 1.0f;
			textureLoaded = true;
		}

	private:
		std::unique_ptr<Surface> pTexture;
		bool textureLoaded = false;
		//Helper variables for clamping
		float tWidth = 0.0f;
		float tHeight = 0.0f;
		float tWidthMax = 0.0f;
		float tHeightMax = 0.0f;
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};