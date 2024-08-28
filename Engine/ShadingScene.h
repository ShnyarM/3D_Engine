#pragma once
#include "Scene.h"
#include "Cube.h"
#include "ScreenTransformer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Mat3.h"
#include "ChiliMath.h"
#include "Pipeline.h"
#include "FlatShadingEffect.h"

class ShadingScene : public Scene
{
public:
	typedef Pipeline<FlatShadingEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	ShadingScene(Graphics& gfx)
		:
		pipeline(gfx),
		itList(Cube::GetPlain<Vertex>(1.0f))
	{
		pipeline.effect.gs.SetSurfaceColor(Colors::White);
		pipeline.effect.gs.SetLightColor(Colors::Magenta);
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

		//Light Movement
		if (kbd.KeyIsPressed('F'))
		{
			theta_light_y -= wrap_angle(dTheta * dt);
		}
		if (kbd.KeyIsPressed('H'))
		{
			theta_light_y += wrap_angle(dTheta * dt);
		}
		if (kbd.KeyIsPressed('T'))
		{
			theta_light_z += wrap_angle(dTheta * dt);
		}
		if (kbd.KeyIsPressed('G'))
		{
			theta_light_z -= wrap_angle(dTheta * dt);
		}
	}

	void ComposeFrame() override
	{
		pipeline.BeginFrame();
		Mat3 rot = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);
		Mat3 rotLight = Mat3::RotationX(theta_light_x) * Mat3::RotationY(theta_light_y) * Mat3::RotationZ(theta_light_z);
		pipeline.effect.vs.BindRotation(rot);
		pipeline.effect.vs.BindTranslation(cubeOffset);
		pipeline.effect.gs.SetLightDir(lightDir * rotLight);
		pipeline.Draw(itList);
	}

private:
	Pipeline pipeline;
	IndexedTriangleList<Vertex> itList;
	float dTheta = 1.4f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;

	Vec3 lightDir = { 0.0f, 0.0f, 1.0f };
	float theta_light_x = 0.0f;
	float theta_light_y = 0.0f;
	float theta_light_z = 0.0f;

	float moveSpeed = 0.02f;
	Vec3 cubeOffset = { 0.0f, 0.0f, 2.0f };
};