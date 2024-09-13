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
#include "CameraController.h"

class SpecularScene : public Scene
{
public:
	SpecularScene(Graphics& gfx, MainWindow& wnd, const char* name, const std::wstring& filename)
		:
		Scene(name),
		gfx(gfx),
		pZb(std::make_unique<ZBuffer>(gfx.ScreenWidth, gfx.ScreenHeight)),
		pipeline(gfx, pZb),
		solidColorPipeline(gfx, pZb),
		monke(ObjectLoader::LoadObjectNormal<BlendNormalVertex>(filename)),
		lightSphere(Sphere::GetPlain<SolidColorVertex>(0.05f, 32, 32)),
		camControl(wnd.mouse, wnd.kbd)
	{
		Color c = Colors::White;
		pipeline.effect.ps.SetSurfaceColor(Colors::White);
		pipeline.effect.ps.SetLightColor(c);

		const Mat4 projection = Mat4::ProjectionFOV(fov, (float)Graphics::ScreenWidth/(float)Graphics::ScreenHeight, nearPlane, farPlane);
		pipeline.effect.vs.BindProjection(projection);
		solidColorPipeline.effect.vs.BindProjection(projection);

		camControl.SetMoveSpeed(camMoveSpeed);
		camControl.SetSensitivity(camSensitity);

		for (auto i = lightSphere.vertices.begin(); i != lightSphere.vertices.end(); i++)
		{
			i->color = c;
		}
	}

	void UpdateModel(Keyboard& kbd, Mouse& mouse, float dt) override
	{
		camControl.UpdateCam(dt);

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
		Mat4 camView = camControl.cam.GetViewTransform();
		pipeline.BeginFrame();

		pipeline.effect.vs.BindWorldTransformation(Mat4::Translation(monkePos));
		pipeline.effect.vs.BindView(camView);
		pipeline.effect.ps.SetLightPos(lightPos);
		pipeline.effect.ps.SetCamView(camView);
		pipeline.Draw(monke);

		solidColorPipeline.effect.vs.BindWorldTransformation(Mat4::Translation(lightPos));
		solidColorPipeline.effect.vs.BindView(camView);
		solidColorPipeline.Draw(lightSphere);
	}

private:
	Graphics& gfx;
	std::shared_ptr<ZBuffer> pZb;
	Pipeline<SpecularPixelPointEffect> pipeline;
	Pipeline<SolidColorEffect> solidColorPipeline;

	static constexpr float fov = 90.0f;
	static constexpr float nearPlane = 0.25f;
	static constexpr float farPlane = 10.0f;

	IndexedTriangleList<BlendNormalVertex> monke;
	IndexedTriangleList<SolidColorVertex> lightSphere;
	Vec3 lightPos = { -1.0f, 0.0f, 1.0f };
	Vec3 monkePos = { 0.0f, 0.0f, 2.0f };
	float moveSpeed = 1.5f;

	CameraController camControl;
	Vec2 camSensitity = { 0.00002f * fov, 0.00002f * fov };
	static constexpr float camMoveSpeed = 1.5f;
};