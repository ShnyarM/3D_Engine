#pragma once
#include "Vec3.h"
#include "Colors.h"
#include <memory>
#include "Surface.h"
#include "Mat3.h"
#include "Triangle.h"
#include <sstream>
#include "DefaultVertexShader.h"

class ShadingTextureEffect
{
public:
	//input vertex
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec3& pos, const Vec2& tPos)
			:
			pos(pos),
			t(tPos)
		{
		}
		Vertex(const Vec3& pos, const Vertex& v)
			:
			pos(pos),
			t(v.t)
		{
		}
		Vertex& operator=(const Vertex& rhs)
		{
			pos = rhs.pos;
			t = rhs.t;
			return *this;
		}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			t += rhs.t;
			return *this;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			t -= rhs.t;
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
			t *= rhs;
			return *this;
		}
		Vertex operator*(const float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(const float rhs)
		{
			pos /= rhs;
			t /= rhs;
			return *this;
		}
		Vertex operator/(const float rhs) const
		{
			return Vertex(*this) /= rhs;
		}

		Vec3 pos;
		Vec2 t;
	};

	//Define wave vertexShader
	typedef DefaultVertexShader<Vertex> VertexShader;

public:
	class GeometryShader
	{
	public:
		// Vertex with a light level and texture Cord associated
		class Output
		{
		public:
			Output() = default;
			Output(const Vec3& pos, const Vec2& tPos, const Vec3& light)
				:
				pos(pos),
				t(tPos),
				light(light)
			{
			}
			Output(const Vec3& pos, const Output& v)
				:
				pos(pos),
				t(v.t),
				light(v.light)
			{
			}
			Output& operator=(const Output& rhs)
			{
				pos = rhs.pos;
				t = rhs.t;
				light = rhs.light;
				return *this;
			}
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				t += rhs.t;
				return *this;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				t -= rhs.t;
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
				return *this;
			}
			Output operator/(const float rhs) const
			{
				return Output(*this) /= rhs;
			}

			Vec3 pos;
			Vec2 t;
			Vec3 light;
		};

	public:
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

		Triangle<Output> operator()(const Vertex& v0, const Vertex& v1, const Vertex& v2, const size_t index)
		{
			Vec3 normal = ((v1.pos - v0.pos) % (v2.pos - v0.pos)).GetNormalized();
			Vec3 strength = diffuse * std::max(0.0f, -(normal * lightDir)); //Strength which hits surface
			Vec3 light = color.GetHadamard(strength + ambient).Saturate(); // strength + ambient is light which hits surface in total
			return { {v0.pos, v0.t, light}, {v1.pos, v1.t, light}, {v2.pos, v2.t, light} };
		}

	private:
		Vec3 lightDir = { 0.0f, -1.0f, 0.0f }; //Direction of the light
		Vec3 diffuse = { 1.0f, 1.0f, 1.0f }; //color of the light
		Vec3 ambient = { 0.1f, 0.1f, 0.1f }; //ambient light of the scene
		Vec3 color = { 1.0f, 1.0f, 1.0f }; //Color of the triangles
	};

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