#pragma once
#include "Vec3.h"
#include "Vec2.h"
#include "Colors.h"
#include <memory>
#include "Surface.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"
#include "BlendColorVertex.h"

class VertexColorEffect
{
public:
	// define input vertex
	typedef BlendColorVertex Vertex;

	//Define VertexShader
	typedef DefaultVertexShader<Vertex> VertexShader;

	//Define GeometryShader
	typedef DefaultGeometryShader<Vertex> GeometryShader;

	class PixelShader
	{
	public:
		// Input class is Vertex
		// Get Color based of vertex
		Color operator()(const VertexShader::Output& in)
		{
			return Color(in.color);
		}
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};