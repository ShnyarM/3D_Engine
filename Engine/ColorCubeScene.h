#pragma once
#include "Scene.h"
#include "Cubes.h"
#include "WorldSpaceTransformer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Mat3.h"

class ColorCubeScene : public Scene
{
	void UpdateModel(Keyboard& kbd, Mouse& mouse, float dt) override
	{
		//Rotation
		if (kbd.KeyIsPressed('W')) //up
		{
			theta_z += dTheta * dt;
		}
		if (kbd.KeyIsPressed('S')) //down
		{
			theta_z -= dTheta * dt;
		}
		if (kbd.KeyIsPressed('D')) //right
		{
			theta_y -= dTheta * dt;
		}
		if (kbd.KeyIsPressed('A')) //left
		{
			theta_y += dTheta * dt;
		}

		//Cube Movement
		if (kbd.KeyIsPressed('L'))
		{
			cubeOffset.x += moveSpeed;
		}
		if (kbd.KeyIsPressed('J'))
		{
			cubeOffset.x -= moveSpeed;
		}
		if (kbd.KeyIsPressed('I'))
		{
			cubeOffset.z += moveSpeed;
		}
		if (kbd.KeyIsPressed('K'))
		{
			cubeOffset.z -= moveSpeed;
		}
	}

	void ComposeFrame(Graphics& gfx) const override
	{
		Mat3 rot = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);
		auto triangles = cube.GetTriangles();

		for (auto& v : triangles.vertices)
		{
			v *= rot;
			v += cubeOffset;
		}

		for (size_t i = 0, end = triangles.indices.size() / 3; i < end; i++)
		{
			const Vec3& v0 = triangles.vertices[triangles.indices[3 * i]];
			const Vec3& v1 = triangles.vertices[triangles.indices[3 * i + 1]];
			const Vec3& v2 = triangles.vertices[triangles.indices[3 * i + 2]];
			Vec3 normal = (v1 - v0) % (v2 - v0);
			triangles.cullFlags[i] = normal * v0 >= 0;
		}

		for (auto& v : triangles.vertices)
		{
			spaceTransformer.Transform(v);
		}

		for (size_t i = 0, end = triangles.indices.size() / 3; i < end; i++)
		{
			if (triangles.cullFlags[i]) continue;
			gfx.DrawTriangle(triangles.vertices[triangles.indices[i * 3]],
				triangles.vertices[triangles.indices[i * 3 + 1]],
				triangles.vertices[triangles.indices[i * 3 + 2]],
				colors[i/2]);
		}
	}

private:
	WorldSpaceTransformer spaceTransformer;
	Cube cube{ 0.5f };
	float dTheta = 0.7f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;

	float moveSpeed = 0.01f;
	Vec3 cubeOffset = { 0.0f, 0.0f, 2.0f };

	static constexpr Color colors[6] = {
			Colors::White,
			Colors::Blue,
			Colors::Cyan,
			Colors::Gray,
			Colors::Green,
			Colors::Magenta,
	};
};