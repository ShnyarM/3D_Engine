#pragma once
#include "Scene.h"
#include "Cube.h"
#include "ScreenTransformer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Mat.h"
#include "ChiliMath.h"
#include "Pipeline.h"
#include "PixelPointEffect.h"
#include "SolidColorEffect.h"
#include "ObjectLoader.h"

class PointLightScene : public Scene
{
public:
	PointLightScene(Graphics& gfx, const char* name, const std::wstring& filename)
		:
		Scene(name),
		gfx(gfx),
		pZb(std::make_unique<ZBuffer>(gfx.ScreenWidth, gfx.ScreenHeight)),
		pipeline(gfx, pZb),
		solidColorPipeline(gfx, pZb),
		monke(ObjectLoader::LoadObjectCalculateNormals<BlendNormalVertex>(filename)),
		lightSphere(Sphere::GetPlain<SolidColorVertex>(0.05f, 32, 32))
	{
		Color c = Colors::White;
		pipeline.effect.ps.SetSurfaceColor(Colors::White);
		pipeline.effect.ps.SetLightColor(c);

		for (auto i = lightSphere.vertices.begin(); i != lightSphere.vertices.end(); i++)
		{
			i->color = c;
		}
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
			cubeOffset.x += moveSpeed * dt;
		}
		if (kbd.KeyIsPressed('J'))
		{
			cubeOffset.x -= moveSpeed * dt;
		}
		if (kbd.KeyIsPressed('I'))
		{
			cubeOffset.z += moveSpeed * dt;
		}
		if (kbd.KeyIsPressed('K'))
		{
			cubeOffset.z -= moveSpeed * dt;
		}

		//Light Movement
		if (kbd.KeyIsPressed('F'))
		{
			lightPos.x -= moveSpeed * dt;
		}
		if (kbd.KeyIsPressed('H'))
		{
			lightPos.x += moveSpeed * dt;
		}
		if (kbd.KeyIsPressed('T'))
		{
			lightPos.z += moveSpeed * dt;
		}
		if (kbd.KeyIsPressed('G'))
		{
			lightPos.z -= moveSpeed * dt;
		}
	}

	void ComposeFrame() override
	{
		pipeline.BeginFrame();
		Mat3 rot = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);
		pipeline.effect.vs.BindRotation(rot);
		pipeline.effect.vs.BindTranslation(cubeOffset);
		pipeline.effect.ps.SetLightPos(lightPos);
		pipeline.Draw(monke);

		solidColorPipeline.effect.vs.BindTranslation(lightPos);
		solidColorPipeline.Draw(lightSphere);
	}

private:
	Graphics& gfx;
	std::shared_ptr<ZBuffer> pZb;
	Pipeline<PixelPointEffect> pipeline;
	Pipeline<SolidColorEffect> solidColorPipeline;

	IndexedTriangleList<BlendNormalVertex> monke;
	IndexedTriangleList<SolidColorVertex> lightSphere;
	float dTheta = 1.4f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;

	Vec3 lightPos = { 0.0f, 0.0f, 0.5f };

	float moveSpeed = 1.5f;
	Vec3 cubeOffset = { 0.0f, -0.1f, 0.5f };
};