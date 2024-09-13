#pragma once
#include "Vec3.h"
#include "Vec2.h"
#include "Colors.h"
#include <memory>
#include "Surface.h"
#include "BaseVertexShader.h"
#include "DefaultGeometryShader.h"
#include "SolidColorVertex.h"

class SolidColorEffect
{
public:
	typedef SolidColorVertex Vertex;

	//Define VertexShader
	class VertexShader : public BaseVertexShader
	{
	public:
		typedef SolidColorVertex4 Output;

	public:
		Output operator()(const Vertex& input)
		{
			return { Vec4(input.pos)*entireTransform, input.color };
		}
	};

	//Define GeometryShader
	typedef DefaultGeometryShader<VertexShader::Output> GeometryShader;

	class PixelShader
	{
	public:
		// Input class is Vertex
		// Get Color based of vertex
		Color operator()(const VertexShader::Output& in)
		{
			return in.color;
		}
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};