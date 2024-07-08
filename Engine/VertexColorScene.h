#pragma once
#include "Scene.h"
#include "Cube.h"
#include "ScreenTransformer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Mat3.h"
#include "ChiliMath.h"
#include "Pipeline.h"
#include "VertexColorEffect.h"

class VertexColorScene : public Scene
{
public:
	typedef Pipeline<VertexColorEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	VertexColorScene(Graphics& gfx)
		:
		pipeline(gfx),
		itList(Cube::GetPlain<Vertex>(1.0f))
	{
		itList.vertices[0].color = Vec3(Colors::Red);
		itList.vertices[1].color = Vec3(Colors::Blue);
		itList.vertices[2].color = Vec3(Colors::White);
		itList.vertices[3].color = Vec3(Colors::Green);
		itList.vertices[4].color = Vec3(Colors::Magenta);
		itList.vertices[5].color = Vec3(Colors::Cyan);
		itList.vertices[6].color = Vec3(Colors::Gray);
		itList.vertices[7].color = Vec3(Colors::Black);
	}

	void UpdateModel(Keyboard& kbd, Mouse& mouse, float dt) override
	{
		//Rotation
		if (kbd.KeyIsPressed('W')) //up
		{
			theta_z += wrap_angle(dTheta * dt);
		}
		if (kbd.KeyIsPressed('S')) //down
		{
			theta_z -= wrap_angle(dTheta * dt);
		}
		if (kbd.KeyIsPressed('D')) //right
		{
			theta_y -= wrap_angle(dTheta * dt);
		}
		if (kbd.KeyIsPressed('A')) //left
		{
			theta_y += wrap_angle(dTheta * dt);
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

	void ComposeFrame() override
	{
		pipeline.BeginFrame();
		Mat3 rot = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);
		pipeline.BindRotation(rot);
		pipeline.BindTranslation(cubeOffset);
		pipeline.Draw(itList);
	}

private:
	Pipeline pipeline;
	IndexedTriangleList<Vertex> itList;
	float dTheta = 0.7f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;

	float moveSpeed = 0.01f;
	Vec3 cubeOffset = { 0.0f, 0.0f, 2.0f };
};
