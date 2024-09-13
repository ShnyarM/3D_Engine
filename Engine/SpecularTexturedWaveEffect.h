#pragma once
#include "TextureBlendNormalVertex.h"
#include "BaseVertexShader.h"
#include "DefaultGeometryShader.h"
#include "BaseSpecularShader.h"
#include "Vec4.h"
#include <memory>
#include "Surface.h"

struct PointDiffuse
{
	float linearAttenuation = 0.3f;
	float quadraticAttenuation = 0.75f;
	float constantAttenuation = 0.4f;
};

class SpecularTexturedWaveEffect
{
public:
	//input vertex
	typedef TextureBlendNormalVertex Vertex;

	class VertexShader : public BaseVertexShader
	{
	public:
		typedef TextureBlendNormalVertexWorld4 Output;

	public:
		void SetTime(float newTime)
		{
			time = newTime;
		}
		Output operator()(const Vertex& input)
		{
			Vec4 newPos(input.pos);

			float sinValue = sin(newPos.x * scale + time * speed) * amplitude;
			float derivative = cos(newPos.x * scale + time * speed) * amplitude * scale;
			newPos.y += sinValue;
			Vec3 newNormal = { 1.0f, -1.0f / derivative, 0.0f };
			newNormal.Normalize();

			Vec3 worldPos = newPos * entireWorldTransform;
			newPos *= entireTransform;

			return { newPos, input.t, Vec4(newNormal, 0.0f)*entireWorldTransform, worldPos };
		}

	private:
		float time = 0.0f;
		static constexpr float amplitude = 0.1f;
		static constexpr float speed = 4.0f;
		static constexpr float scale = 10.0f;
	};

	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

	class PixelShader : public BaseSpecularShader<PointDiffuse>
	{
	public:
		// Get Color based of vertex and current loaded texture
		Color operator()(const VertexShader::Output& in)
		{
			Vec3 light = LightLevel(in);
			assert(textureLoaded);
			Vec3 pixelColor = Vec3(pTexture->GetPixel(
				(unsigned int)std::min(std::max(in.t.x * tWidth, 0.0f), tWidthMax),
				(unsigned int)std::min(std::max(in.t.y * tHeight, 0.0f), tHeightMax)));
			pixelColor.x *= light.x;
			pixelColor.y *= light.y;
			pixelColor.z *= light.z;
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
