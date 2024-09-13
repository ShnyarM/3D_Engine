#pragma once
#include "Vec3.h"
#include "Mat.h"

class Camera
{
public:
	void SetPos(const Vec3& newPos)
	{
		pos = newPos;
	}
	void SetRot(const Vec3& newRot)
	{
		rot = newRot;
	}

	void AddToPos(const Vec3& add)
	{
		pos += add;
	}
	void AddToPosX(float add)
	{
		pos.x += add;
	}
	void AddToPosY(float add)
	{
		pos.y += add;
	}
	void AddToPosZ(float add)
	{
		pos.z += add;
	}

	void AddToRot(const Vec3& add)
	{
		rot += add;
	}
	Vec3 GetRot() const
	{
		return rot;
	}

	void ClampZ()
	{
		rot.z = std::max(-PI/3.0f, std::min(PI / 3.0f, rot.z));
	}

	Mat4 GetViewTransform() const
	{
		return Mat4::Translation(-pos) * Mat4::RotationY(-rot.y) * Mat4::RotationX(-rot.x) * Mat4::RotationZ(-rot.z);
	}
	Mat4 GetRotTransform() const
	{
		return Mat4::RotationY(rot.y) * Mat4::RotationX(rot.x) * Mat4::RotationZ(rot.z);
	}
private:
	Vec3 pos = { 0.0f, 0.0f, 0.0f };
	Vec3 rot = { 0.0f, 0.0f, 0.0f };
};
