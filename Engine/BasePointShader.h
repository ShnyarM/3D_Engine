#pragma once
#include "Vec4.h"
#include "Colors.h"
#include "Mat.h"
#include "BaseSpecularShader.h"

template<class PointDiffuse = DefaultPointDiffuse>
class BasePointShader
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
		transformedLightPos = Vec4(lightPos) * camViewTransform;
	}
	void SetLightColor(const Color& newColor)
	{
		lightColor = Vec3(newColor) / 255.0f;
	}
	void SetAmbient(const Vec3& newAmbient)
	{
		ambient = newAmbient;
	}
	void SetCamView(const Mat4& view)
	{
		camViewTransform = view;
		transformedLightPos = Vec4(lightPos) * camViewTransform;
	}

	// Calculates the light level which hits the object
	// Here light is already transformed by camera view
	template<class Input>
	Vec3 LightLevel(const Input& input)
	{
		Vec3 normal = input.n.GetNormalized();

		// Get information about position relative to light
		Vec3 diffVec = transformedLightPos - input.worldPos;
		float distance = diffVec.Len();
		Vec3 diffVecN = diffVec.GetNormalized();

		float attenuation = 1.0f / (diffuseValues.constantAttenuation + distance * diffuseValues.linearAttenuation + sq(distance) * diffuseValues.quadraticAttenuation); //distance factor, less if further away
		Vec3 strength = lightColor * attenuation * std::max(0.0f, (normal * diffVecN)); //Strength which hits surface
		Vec3 cVec = color.GetHadamard(strength + ambient).Saturate(); // strength + ambient is light which hits surface in total
		return cVec;
	}

	// Gives back a color which is color of material at said pixel
	// Here light is already transformed by camera view
	template<class Input>
	Color Shade(const Input& input)
	{
		return Color(LightLevel(input) * 255.0f);
	}

private:
	Vec3 lightPos = { 0.0f, 0.0f, 0.5f }; //position of the light in the world
	Vec3 lightColor = { 1.0f, 1.0f, 1.0f }; //color of the light
	Vec3 ambient = { 0.05f, 0.05f, 0.05f }; //ambient light of the scene
	Vec3 color = { 1.0f, 1.0f, 1.0f }; //Color of the material

	Mat4 camViewTransform = Mat4::Identity();
	Vec3 transformedLightPos = lightPos; //position of the light in the world transformed by camera view

	PointDiffuse diffuseValues;
};
