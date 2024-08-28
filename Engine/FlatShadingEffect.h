#pragma once
#include "DefaultVertex.h"
#include "DefaultVertexShader.h"
#include "Colors.h"
#include "Triangle.h"
#include <utility>

class FlatShadingEffect
{
public:
	// Define Input Vertex
	typedef DefaultVertex Vertex;

	//Define VertexShader
	typedef DefaultVertexShader<Vertex> VertexShader;

public:
	class GeometryShader
	{
	public:
		// Vertex with a color associated
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
			Vec3 cVec = color.GetHadamard(strength + ambient).Saturate() * 255.0f; // strength + ambient is light which hits surface in total
			Color c(cVec);
			return { {v0.pos, c}, {v1.pos, c}, {v2.pos, c} };
		}

	private:
		Vec3 lightDir = { 0.0f, 0.0f, 1.0f }; //Direction of the light
		Vec3 diffuse = { 1.0f, 1.0f, 1.0f }; //color of the light
		Vec3 ambient = { 0.1f, 0.1f, 0.1f }; //ambient light of the scene
		Vec3 color = { 0.8f, 0.8f, 0.9f }; //Color of the triangles
	};

public:
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
