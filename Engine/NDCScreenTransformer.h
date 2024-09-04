#pragma once
#include "Graphics.h"
#include "Vec3.h"

class NDCScreenTransformer
{
public:
	NDCScreenTransformer()
		:
		xFactor( float(Graphics::ScreenWidth) / 2.0f ),
		yFactor( float(Graphics::ScreenHeight) / 2.0f )
	{}
	
	// takes a vertex which is in homogenous space (x, y have no perspective applied, z is between 0 and far and w has z coordinate)
	// convert from homogenous to ndc space and a transform to screen coordinates
	template <class Vertex>
	Vertex& Transform(Vertex& v) const
	{
		// at this point, w stores original z coordinate
		const float wInverse = 1.0f / v.pos.w;

		//Multiply everything by wInverse, i.e. apply perspective
		//Makes interpolation linear again and thus removes distortion
		v *= wInverse;
		
		//go from -1 and 1 to screenwidth and height
		v.pos.x = (v.pos.x + 1) * xFactor;
		v.pos.y = (-v.pos.y + 1) * yFactor;

		//Store 1/z in z, so we can recover original uv coordinates again later
		//Interpolation will be performed correctly on 1/z
		v.pos.w = wInverse;

		return v;
	}

	template <class Vertex>
	Vertex GetTransformed(const Vertex& v) const
	{
		return Transform( Vertex( v ) );
	}

private:
	//float halfWidth = 2.0f;
	//float halfH eight = 2.0f;
	float xFactor;
	float yFactor;
};