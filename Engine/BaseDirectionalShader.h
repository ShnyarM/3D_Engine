#pragma once
#include "Vec3.h"
#include "Colors.h"
#include "Mat.h"

class BaseDirectionalShader
{
public:
	// light functions
	void SetSurfaceColor(const Color& newColor)
	{
		color = Vec3(newColor) / 255.0f;
	}
	void SetLightDir(const Vec3& newDir)
	{
		lightDir = newDir.GetNormalized();
		transformedLightDir = Vec4(lightDir, 0.0f) * camViewTransform; //w attribute is zero so it only gets rotated
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
		transformedLightDir = Vec4(lightDir, 0.0f) * camViewTransform; //w attribute is zero so it only gets rotated
	}

	// Calculates the light level which hits the object
	// Here light is already transformed by camera view
	template<class Input>
	Vec3 LightLevel(const Input& input)
	{
		Vec3 normal = input.n.GetNormalized();

		Vec3 strength = lightColor * std::max(0.0f, -(normal * transformedLightDir)); //Strength which hits surface
		Vec3 cVec = color.GetHadamard(strength + ambient).Saturate() * 255.0f; // strength + ambient is light which hits surface in total
		return Color(cVec);
	}

	// Gives back a color which is color of material at said pixel
	// Here light is already transformed by camera view
	template<class Input>
	Color Shade(const Input& input)
	{
		return Color(LightLevel(input) * 255.0f);
	}

private:
	Vec3 lightDir = { 0.0f, 0.0f, 1.0f }; //direction of the light
	Vec3 lightColor = { 1.0f, 1.0f, 1.0f }; //color of the light
	Vec3 ambient = { 0.05f, 0.05f, 0.05f }; //ambient light of the scene
	Vec3 color = { 0.8f, 0.8f, 0.9f }; //Color of the triangles

	Mat4 camViewTransform = Mat4::Identity();
	Vec3 transformedLightDir = lightDir; //position of the light in the world transformed by camera view
};
