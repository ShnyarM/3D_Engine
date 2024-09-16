#pragma once
#include "Scene.h"
#include "Sphere.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Mat.h"
#include "ChiliMath.h"
#include "Pipeline.h"
#include "SpecularPixelPointEffect.h"
#include "SolidColorEffect.h"
#include "ObjectLoader.h"

class SpecularScene : public Scene
{
public:
	SpecularScene(Graphics& gfx, const char* name)
		:
		Scene(name),
		gfx(gfx),
		pZb(std::make_unique<ZBuffer>(gfx.ScreenWidth, gfx.ScreenHeight)),
		pipeline(gfx, pZb),
		solidColorPipeline(gfx, pZb),
		monke(ObjectLoader::LoadObjectNormal<BlendNormalVertex>(L"models/suzanne.obj")),
		lightSphere(Sphere::GetPlain<SolidColorVertex>(0.05f, 32, 32))
	{
		Color c = Colors::White;
		pipeline.effect.ps.SetSurfaceColor(Colors::White);
		pipeline.effect.ps.SetLightColor(c);

		const Mat4 projection = Mat4::ProjectionFOV(fov, (float)Graphics::ScreenWidth / (float)Graphics::ScreenHeight, nearPlane, farPlane);
		pipeline.effect.vs.BindProjection(projection);
		solidColorPipeline.effect.vs.BindProjection(projection);

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

		//monke Movement
		if (kbd.KeyIsPressed('L'))
		{
			monkePos.x += moveSpeed * dt;
		}
		if (kbd.KeyIsPressed('J'))
		{
			monkePos.x -= moveSpeed * dt;
		}
		if (kbd.KeyIsPressed('I'))
		{
			monkePos.z += moveSpeed * dt;
		}
		if (kbd.KeyIsPressed('K'))
		{
			monkePos.z -= moveSpeed * dt;
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

		Mat4 monkeTransform = Mat4::RotationX(theta_x) * Mat4::RotationY(theta_y) * Mat4::RotationZ(theta_z) * Mat4::Translation(monkePos);
		pipeline.effect.vs.BindWorldTransformation(monkeTransform);
		pipeline.effect.ps.SetLightPos(lightPos);
		pipeline.Draw(monke);

		solidColorPipeline.effect.vs.BindWorldTransformation(Mat4::Translation(lightPos));
		solidColorPipeline.Draw(lightSphere);

		gfx.DrawText("Use (W,A,S,D), (T,F,G,H) and (I,J,K,L) to move the objects", 0, Graphics::ScreenHeight - gfx.GetFont().GetCHeight());
	}

private:
	Graphics& gfx;
	std::shared_ptr<ZBuffer> pZb;
	Pipeline<SpecularPixelPointEffect> pipeline;
	Pipeline<SolidColorEffect> solidColorPipeline;

	static constexpr float fov = 90.0f;
	static constexpr float nearPlane = 0.25f;
	static constexpr float farPlane = 10.0f;

	static constexpr float dTheta = 1.4f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;

	IndexedTriangleList<BlendNormalVertex> monke;
	IndexedTriangleList<SolidColorVertex> lightSphere;
	Vec3 lightPos = { -1.0f, 0.0f, 1.0f };
	Vec3 monkePos = { 0.0f, 0.0f, 3.0f };
	static constexpr float moveSpeed = 1.5f;
};