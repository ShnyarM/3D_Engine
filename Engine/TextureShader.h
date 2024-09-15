#pragma once
#include "TextureVertex.h"
#include "BaseVertexShader.h"
#include "Vec4.h"
#include "DefaultGeometryShader.h"
#include "BaseTextureShader.h"

class TextureShader
{
public:
	typedef TextureVertex Vertex;

	class VertexShader : public BaseVertexShader
	{
	public:
		typedef TextureVertex4 Output;

	public:
		Output operator()(const Vertex& in)
		{
			return { Vec4(in.pos) * entireTransform, in.t };
		}
	};

	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

	class PixelShader : public BaseTextureShader
	{
	public:
		Color operator()(const TextureVertex4& in)
		{
			return GetColor(in);
		}
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};