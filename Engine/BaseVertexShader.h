#pragma once

//VertexShader template which has projection, view and world transform
//Templated on input vertex type of effect, so it can be used with any effect
class BaseVertexShader
{
public:
	void BindWorldTransformation(const Mat4& transform)
	{
		worldTransform = transform;
		UpdateTransform();
	}
	void BindProjection(const Mat4& projection)
	{
		worldProjection = projection;
		UpdateTransform();
	}
	void BindView(const Mat4& view)
	{
		worldView = view;
		UpdateTransform();
	}
	const Mat4 GetProj() const
	{
		return worldProjection;
	}
private:
	void UpdateTransform()
	{
		entireWorldTransform = worldTransform * worldView;
		entireTransform = entireWorldTransform * worldProjection;
	}

protected:
	Mat4 worldTransform = Mat4::Identity();
	Mat4 worldProjection = Mat4::Identity();
	Mat4 worldView = Mat4::Identity();

	Mat4 entireWorldTransform = Mat4::Identity(); // transform which contains world transform and camera transform
	Mat4 entireTransform = Mat4::Identity(); // transform which containg world transform, camera transform and projection
};