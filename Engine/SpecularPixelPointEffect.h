#pragma once
#include "BlendNormalVertex.h"
#include "DefaultGeometryShader.h"
#include "Colors.h"
#include "Triangle.h"
#include <utility>
#include <cmath>
#include "BlendColorVertex.h"
#include "Mat.h"
#include "BaseVertexShader.h"
#include "BaseSpecularShader.h"
#include "WorldNormalVertex.h"

class SpecularPixelPointEffect
{
public:
	// Define Input Vertex
	typedef BlendNormalVertex Vertex;

	// Define VertexShader, default vertexShader but rotates normal
	class VertexShader : public BaseVertexShader
	{
	public:
		// normal blendNormalVertex, but add original worldposition as additional information for pixelshader
		typedef WorldNormalVertex4 Output;

	public:
		Output operator()(const Vertex& input)
		{
			return { Vec4(input.pos) * entireTransform, Vec4(input.n, 0.0f) * entireWorldTransform, Vec4(input.pos) * entireWorldTransform };
		}
	};

public:
	// Default GeometryShader
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

public:
	class PixelShader : public BaseSpecularShader<>
	{
	public:
		Color operator()(const VertexShader::Output& input)
		{
			return this->Shade(input);
		}
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};
