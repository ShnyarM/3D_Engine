#pragma once
#include "NormalVertex.h"
#include "DefaultGeometryShader.h"
#include "Colors.h"
#include "Triangle.h"
#include <utility>
#include "BlendColorVertex.h"
#include "Mat3.h"

class GouraudPointEffect
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

			// Get information about position relative to light
			Vec3 diffVec = lightPos - newPos;
			float distance = diffVec.Len();
			diffVec.Normalize();

			float attenuation = 1.0f / (constantAttenuation + distance * linearAttenuation + sq(distance) * quadraticAttenuation); //distance factor, less if further away
			Vec3 strength = diffuse * attenuation * std::max(0.0f, (normal * diffVec)); //Strength which hits surface
			Vec3 cVec = color.GetHadamard(strength + ambient).Saturate() * 255.0f; // strength + ambient is light which hits surface in total
			return { newPos, cVec };
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
