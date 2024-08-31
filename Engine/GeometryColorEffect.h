#pragma once
#include "DefaultVertex.h"
#include "DefaultVertexShader.h"
#include "Colors.h"
#include "Triangle.h"
#include <vector>
#include "SolidColorVertex.h"


class GeometryColorEffect
{
public:
	// Define Input Vertex
	typedef DefaultVertex Vertex;

	// Define VertexShader
	typedef DefaultVertexShader<Vertex> VertexShader;

public:
	class GeometryShader
	{
	public:
		//Vertex with color associated
		typedef SolidColorVertex Output;

		void BindColors(std::vector<Color> incoming)
		{
			colors = incoming;
		}

		Triangle<Output> operator()(const Vertex& v0, const Vertex& v1, const Vertex& v2, const size_t index)
		{
			Color& colorToUse = colors[index / 2];
			return { {v0.pos, colorToUse}, {v1.pos, colorToUse}, {v2.pos, colorToUse} };
		}

	private:
		std::vector<Color> colors;
	};

	class PixelShader
	{
		typedef GeometryShader::Output Input;

	public:
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