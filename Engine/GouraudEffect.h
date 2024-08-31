#pragma once
#include "NormalVertex.h"
#include "DefaultGeometryShader.h"
#include "Colors.h"
#include "Triangle.h"
#include <utility>
#include "BlendColorVertex.h"

class GouraudEffect
{
public:
	// Define Input Vertex
	typedef NormalVertex Vertex;

	// Define VertexShader
	class VertexShader
	{
	public:
		//Define output
		typedef BlendColorVertex Output;

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
			Vec3 cVec = color.GetHadamard(strength + ambient).Saturate() * 255.0f; // strength + ambient is light which hits surface in total
			return { newPos, cVec };
		}

	private:
		Vec3 lightDir = { 0.0f, 0.0f, 1.0f }; //Direction of the light
		Vec3 diffuse = { 1.0f, 1.0f, 1.0f }; //color of the light
		Vec3 ambient = { 0.1f, 0.1f, 0.1f }; //ambient light of the scene
		Vec3 color = { 0.8f, 0.8f, 0.9f }; //Color of the triangles

		Vec3 translation;
		Mat3 rotation = Mat3::Identity();
	};

public:
	// Default GeometryShader
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

public:
	class PixelShader
	{
		typedef GeometryShader::Output Input;
	public:
		Color operator()(const Input& in)
		{
			return Color(in.color);
		}
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};
