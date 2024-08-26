#pragma once
#include "Triangle.h"

//Default geometry shader which does nothing
//Templated on input vertex type of effect, so it can be used with any effect
template <typename Vertex>
class DefaultGeometryShader
{
public:
	//Set vertex class as input class, will be VSOut
	typedef Vertex Output;
public:
	Triangle<Output> operator()(const Vertex& v0, const Vertex& v1, const Vertex& v2, const size_t index)
	{
		return { v0, v1, v2 };
	}

private:
	Vec3 translation;
	Mat3 rotation = Mat3::Identity();
};