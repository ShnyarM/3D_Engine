#pragma once
#include "Vec3.h"
#include "Colors.h"
#include <memory>
#include "Surface.h"
#include "Mat.h"
#include "Triangle.h"
#include <sstream>
#include "DefaultGeometryShader.h"
#include "TextureNormalVertex.h"
#include "BlendColorVertex.h"

class TexturedGouraudEffect
{
public:
	//input vertex
	typedef TextureNormalVertex Vertex;

	// Define VertexShader
	class VertexShader
	{
	public:
		class Output
		{
		public:
			Output() = default;
			Output(const Vec3& pos, const Vec2& tPos, const Vec3& normal, const Vec3& light)
				:
				pos(pos),
				t(tPos),
				n(normal),
				light(light)
			{
			}
			Output(const Vec3& pos, const Output& v)
				:
				pos(pos),
				t(v.t),
				n(v.n),
				light(v.light)
			{
			}
			Output& operator=(const Output& rhs)
			{
				pos = rhs.pos;
				t = rhs.t;
				n = rhs.n;
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

			Vec3 pos;
			Vec2 t;
			Vec3 n;
			Vec3 light;
		};

	public:
		// light functions
		void SetSurfaceColor(const Color& newColor)
		{
			color = Vec3(newColor) / 255.0f;
		}
		void SetLightDir(const Vec3& newDir)
		{
			lightDir = newDir.GetNormalized();
		}
		void SetLightColor(const Color& newColor)
		{
			diffuse = Vec3(newColor) / 255.0f;
		}
		void SetAmbient(const Vec3& newAmbient)
		{
			ambient = newAmbient;
		}

		//default vertexshader functions
		void BindRotation(const Mat3& rotation_in)
		{
			rotation = rotation_in;
		}
		void BindTranslation(const Vec3& translation_in)
		{
			translation = translation_in;
		}

		// add color to vertex (removes vertex in process since blendcolorVertex doesnt have normal)
		Output operator()(const Vertex& input)
		{
			Vec3 newPos = (input.pos * rotation) + translation;
			Vec3 normal = (input.n * rotation); //rotated normal

			Vec3 strength = diffuse * std::max(0.0f, -(normal * lightDir)); //Strength which hits surface
			Vec3 light = color.GetHadamard(strength + ambient).Saturate(); // strength + ambient is light which hits surface in total
			return { newPos, input.t, input.n, light };
		}

	private:
		Vec3 lightDir = { 0.0f, 0.0f, 1.0f }; //Direction of the light
		Vec3 diffuse = { 1.0f, 1.0f, 1.0f }; //color of the light
		Vec3 ambient = { 0.1f, 0.1f, 0.1f }; //ambient light of the scene
		Vec3 color = { 0.8f, 0.8f, 0.9f }; //Color of the triangles

		Vec3 translation;
		Mat3 rotation = Mat3::Identity();
	};

	//Define GeometrxShader
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

	class PixelShader
	{
		typedef GeometryShader::Output Input;
	public:
		// Get Color based of vertex and current loaded texture
		Color operator()(const Input& in)
		{
			assert(textureLoaded);
			Vec3 pixelColor = Vec3(pTexture->GetPixel(
				(unsigned int)std::min(in.t.x * tWidth, tWidthMax),
				(unsigned int)std::min(in.t.y * tHeight, tHeightMax)));
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