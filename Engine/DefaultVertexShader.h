#pragma once

//Default vertex shader which applies rotation and translation
//Templated on input vertex type of effect, so it can be used with any effect
template <typename Vertex>
class DefaultVertexShader
{
public:
	//Set vertex class as input class
	typedef Vertex Output;
public:
	void BindRotation(const Mat3& rotation_in)
	{
		rotation = rotation_in;
	}
	void BindTranslation(const Vec3& translation_in)
	{
		translation = translation_in;
	}
	Output operator()(const Vertex& input)
	{
		return Vertex{ (input.pos * rotation) + translation, input };
	}

private:
	Vec3 translation;
	Mat3 rotation = Mat3::Identity();
};