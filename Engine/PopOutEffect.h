#pragma once
#include "TextureEffect.h"


class PopOutEffect
{
public:
	// Set InputVertex
	typedef TextureEffect::Vertex Vertex;

	// Set pixelshader
	typedef TextureEffect::PixelShader PixelShader;

	// Set VertexShader
	typedef DefaultVertexShader<Vertex> VertexShader;

public:
	class GeometryShader
	{
	public:
		// Set Output Vertex
		typedef Vertex Output;

	public:
		void SetTime(const float newTime)
		{
			time = newTime;
		}

		Triangle<Vertex> operator()(const Vertex& v0, const Vertex& v1, const Vertex& v2, const size_t index)
		{
			Vec3 normal = (v1.pos - v0.pos) % (v2.pos - v0.pos).Normalize();
			Vec3 toApply = normal * (sin(time * speed) + 1) * amplitude;
			return { {v0.pos + toApply, v0}, {v1.pos + toApply, v1}, {v2.pos + toApply, v2} };
		}

	public:
		float time = 0.0f;
		const float amplitude = 0.1f;
		const float speed = 4.0f;
		const float scale = 10.0f;
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};