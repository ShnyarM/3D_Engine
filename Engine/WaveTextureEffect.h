#pragma once
#include "Vec3.h"
#include "Colors.h"
#include <memory>
#include "Surface.h"
#include "Mat3.h"
#include "TextureEffect.h"

class WaveTextureEffect
{
public:
	//use textureeffect vertices as input vertices
	typedef TextureEffect::Vertex Vertex;
	
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
			float sinValue = sin(newPos.x + time * speed) * amplitude;
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
	};

	//Use textureeffect pixelshader as pixelshader
	typedef TextureEffect::PixelShader PixelShader;
	
public:
	VertexShader vs;
	PixelShader ps;
};