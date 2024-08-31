#pragma once
#include "Vec3.h"
#include "Vec2.h"
#include "Colors.h"
#include <memory>
#include "Surface.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"
#include "SolidColorVertex.h"

class SolidColorEffect
{
public:
	typedef SolidColorVertex Vertex;

	//Define VertexShader
	typedef DefaultVertexShader<Vertex> VertexShader;

	//Define GeometryShader
	typedef DefaultGeometryShader<Vertex> GeometryShader;

	class PixelShader
	{
	public:
		// Input class is Vertex
		// Get Color based of vertex
		template <class Input>
		Color operator()(const Input& in)
		{
			return in.color;
		}
	};

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};