#pragma once
#include "Scene.h"
#include "Cubes.h"
#include "WorldSpaceTransformer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Mat3.h"

class WireframeCubeScene : public Scene
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
		auto lines = cube.GetLines();

		for (auto& v : lines.vertices)
		{
			v *= rot;
			v += cubeOffset;
		}


		for (auto& v : lines.vertices)
		{
			spaceTransformer.Transform(v);
		}

		for (size_t i = 0, end = lines.indices.size() / 2; i < end; i++)
		{
			gfx.DrawLine(lines.vertices[lines.indices[i * 2]], lines.vertices[lines.indices[i * 2 + 1]], Colors::White);
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
};
