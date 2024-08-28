#pragma once
#include "Scene.h"
#include "Cube.h"
#include "ScreenTransformer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Mat3.h"
#include "ChiliMath.h"
#include "Pipeline.h"
#include "GeometryColorEffect.h"

class TwoCubesScene : public Scene
{
public:
	typedef Pipeline<GeometryColorEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	TwoCubesScene(Graphics& gfx)
		:
		pipeline(gfx),
		itList1(Cube::GetPlain<Vertex>(1.0f)),
		itList2(Cube::GetPlain<Vertex>(1.0f))
	{
		std::vector<Color> colors = { Colors::Red, Colors::Blue, Colors::White, Colors::Green, Colors::Cyan, Colors::Magenta };
		pipeline.effect.gs.BindColors(std::move(colors));
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
		//Draw first cube with fixed offset
		Mat3 rot = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);
		pipeline.effect.vs.BindRotation(rot);
		pipeline.effect.vs.BindTranslation({ 0.0f, 0.0f, 2.0f });
		pipeline.Draw(itList1);

		//Draw second cube with different rotation and fixed offset
		rot = Mat3::RotationX(-theta_x) * Mat3::RotationY(-theta_y) * Mat3::RotationZ(-theta_z);
		pipeline.effect.vs.BindRotation(rot);
		pipeline.effect.vs.BindTranslation(cubeOffset);
		pipeline.Draw(itList2);
	}

private:
	Pipeline pipeline;
	IndexedTriangleList<Vertex> itList1;
	IndexedTriangleList<Vertex> itList2;
	float dTheta = 0.7f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;

	float moveSpeed = 0.01f;
	Vec3 cubeOffset = { 0.0f, 0.0f, 2.0f };
};