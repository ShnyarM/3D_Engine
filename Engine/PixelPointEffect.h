#pragma once
#include "BlendNormalVertex.h"
#include "DefaultGeometryShader.h"
#include "Colors.h"
#include "Triangle.h"
#include <utility>
#include "BlendColorVertex.h"
#include "Mat.h"
#include "DefaultVertexShader.h"

class PixelPointEffect
{
public:
	// Define Input Vertex
	typedef BlendNormalVertex Vertex;

	// Define VertexShader, default vertexShader but rotates normal
	class VertexShader
	{
	public:
		// normal blendNormalVertex, but add original worldposition as additional information for pixelshader
		class Output
		{
		public:
			Output() = default;
			Output(const Vec3& pos, const Vec3& normal, const Vec3& worldPos)
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

			Vec3 pos;
			Vec3 n;
			Vec3 worldPos;
		};

	public:
		void BindRotation(const Mat3& rotation_in)
		{
			rotation = rotation_in;
		}
		void BindTranslation(const Vec3& translation_in)
		{
			translation = translation_in;
		}
		Output operator()(const Vertex& input)
		{
			Vec3 newPos = (input.pos * rotation) + translation;
			return { newPos, input.n * rotation, newPos };
		}

	private:
		Vec3 translation;
		Mat3 rotation = Mat3::Identity();
	};

public:
	// Default GeometryShader
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

public:
	class PixelShader
	{
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
			diffuse = Vec3(newColor) / 255.0f;
		}
		void SetAmbient(const Vec3& newAmbient)
		{
			ambient = newAmbient;
		}

		// get Color from Vertex
		Color operator()(const VertexShader::Output& input)
		{
			Vec3 normal = input.n.GetNormalized();

			// Get information about position relative to light
			Vec3 diffVec = lightPos - input.worldPos;
			float distance = diffVec.Len();
			diffVec.Normalize();

			float attenuation = 1.0f / (constantAttenuation + distance * linearAttenuation + sq(distance) * quadraticAttenuation); //distance factor, less if further away
			Vec3 strength = diffuse * attenuation * std::max(0.0f, (normal * diffVec)); //Strength which hits surface
			Vec3 cVec = color.GetHadamard(strength + ambient).Saturate() * 255.0f; // strength + ambient is light which hits surface in total
			return Color(cVec);
		}

	private:
		Vec3 lightPos = { 0.0f, 0.0f, 0.5f }; //position of the light
		Vec3 diffuse = { 1.0f, 1.0f, 1.0f }; //color of the light
		Vec3 ambient = { 0.05f, 0.05f, 0.05f }; //ambient light of the scene
		Vec3 color = { 0.8f, 0.8f, 0.9f }; //Color of the triangles

		Vec3 translation;
		Mat3 rotation = Mat3::Identity();

		float linearAttenuation = 1.0f;
		float quadraticAttenuation = 2.62f;
		float constantAttenuation = 0.4f;
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};
