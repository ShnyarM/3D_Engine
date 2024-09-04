#pragma once
#include "Vec3.h"
#include "Colors.h"
#include <memory>
#include "Surface.h"
#include "Mat.h"
#include "TextureEffect.h"
#include "DefaultGeometryShader.h"
#include "TexturePixelShader.h"

class WaveTextureEffect
{
public:
	//use textureeffect vertices as input vertices
	typedef TextureVertex Vertex;

	//Define GeometryShader
	typedef DefaultGeometryShader<Vertex> GeometryShader;
	
	//Define wave vertexShader
	class VertexShader
	{
	public:
		//Set Output vertex as inputvertex
		typedef Vertex Output;

	public:
		void BindRotation(const Mat3& rotation_in)
		{
			rotation = rotation_in;
		}
		void BindTranslation(const Vec3& translation_in)
		{
			translation = translation_in;
		}
		void SetTime(const float newTime)
		{
			time = newTime;
		}
		//First adjust y position with wave function and then apply rotation and translation
		Output operator()(const Vertex& input)
		{
			Vec3 newPos = input.pos;
			float sinValue = sin(newPos.x * scale + time * speed) * amplitude;
			newPos.y += sinValue;
			newPos = (newPos * rotation) + translation;
			return { newPos, input };
		}
	private:
		Vec3 translation;
		Mat3 rotation = Mat3::Identity();
		float time = 0.0f;
		const float amplitude = 0.1f;
		const float speed = 4.0f;
		const float scale = 10.0f;
	};

	//Use textureeffect pixelshader as pixelshader
	typedef TexturePixelShader<Vertex> PixelShader;
	
public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};