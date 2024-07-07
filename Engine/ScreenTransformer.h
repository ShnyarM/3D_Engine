#pragma once
#include "Graphics.h"
#include "Vec3.h"

class ScreenTransformer
{
public:
	ScreenTransformer()
		:
		xFactor( float(Graphics::ScreenWidth) / 2.0f ),
		yFactor( float(Graphics::ScreenWidth) / 2.0f )
	{}

	template <class Vertex>
	Vertex& Transform(Vertex& v) const
	{
		//const float zInverse = screenDistance / v.z;
		const float zInverse = 1.0f / v.pos.z;

		//Multiply everything by zInverse, i.e. apply perspective
		//Makes interpolation linear again and thus removes distortion
		v *= zInverse;
		
		//go from -1 and 1 to screenwidth and height
		v.pos.x = (v.pos.x + 1) * xFactor;
		v.pos.y = (-v.pos.y + 1) * yFactor;

		//Store 1/z in z, so we can recover original uv coordinates again later
		//Interpolation will be performed correctly on 1/z
		v.pos.z = zInverse;

		return v;
	}

	template <class Vertex>
	Vertex GetTransformed(const Vertex& v) const
	{
		return Transform( Vertex( v ) );
	}

	float screenDistance = 1.0f;
private:
	float xFactor;
	float yFactor;
};