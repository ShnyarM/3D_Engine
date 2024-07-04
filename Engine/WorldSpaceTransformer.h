#pragma once
#include "Graphics.h"
#include "Vec3.h"

class WorldSpaceTransformer
{
public:
	WorldSpaceTransformer()
		:
		xFactor( float(Graphics::ScreenWidth) / 2.0f ),
		yFactor( float(Graphics::ScreenWidth) / 2.0f )
	{}

	Vec3& Transform(Vec3& v)
	{
		const float zInverse = screenDistance / v.z;
		v.x = (v.x*zInverse + 1) * xFactor;
		v.y = (-v.y*zInverse + 1) * yFactor;
		return v;
	}

	Vec3 GetTransformed(const Vec3& v)
	{
		return Transform( Vec3( v ) );
	}

	float screenDistance = 1.0f;
private:
	float xFactor;
	float yFactor;
};