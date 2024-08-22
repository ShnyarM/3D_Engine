#pragma once
#include "Scene.h"
#include "Cube.h"
#include "ScreenTransformer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Mat3.h"
#include "ChiliMath.h"
#include "Pipeline.h"
#include "TextureEffect.h"

class TextureCubeScene : public Scene
{
public:
	typedef Pipeline<TextureEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	TextureCubeScene(Graphics& gfx, const std::wstring& filename)
		:
		pipeline(gfx),
		itList(Cube::GetSkinned<Vertex>(1.0f))
	{
		pipeline.effect.ps.BindTexture(filename);
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
		pipeline.effect.vs.BindRotation(rot);
		pipeline.effect.vs.BindTranslation(cubeOffset);
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
