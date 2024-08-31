#pragma once
#include "Vec3.h"
#include "Vec2.h"
#include "Colors.h"
#include <memory>
#include "Surface.h"
#include "DefaultVertexShader.h"
#include "DefaultGeometryShader.h"
#include "TextureVertex.h"
#include "TexturePixelShader.h"

class TextureEffect
{
public:
	//input vertex
	typedef TextureVertex Vertex;
	
	//Define VertexShader
	typedef DefaultVertexShader<Vertex> VertexShader;

	//Define GeometryShader
	typedef DefaultGeometryShader<Vertex> GeometryShader;

	//define pixelshader
	typedef TexturePixelShader<Vertex> PixelShader;

public:
	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};