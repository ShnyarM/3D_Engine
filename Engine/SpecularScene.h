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
#include "SpecularTexturedWaveEffect.h"
#include "TexturedVertexPointEffect.h"
#include "ObjectLoader.h"
#include "Plane.h"
#include "Player.h"

class SpecularScene : public Scene
{
	// rename pipelines
	typedef Pipeline<SpecularPixelPointEffect> SpecularPipeline;
	typedef Pipeline<SolidColorEffect> SolidColorPipeline;
	typedef Pipeline<SpecularTexturedWaveEffect> WavePipeline;
	typedef Pipeline<TexturedVertexPointEffect> VertexLightPipeline;

public:
	SpecularScene(Graphics& gfx, MainWindow& wnd, const char* name)
		:
		Scene(name),
		gfx(gfx),
		pZb(std::make_unique<ZBuffer>(gfx.ScreenWidth, gfx.ScreenHeight)),
		pipeline(gfx, pZb),
		solidColorPipeline(gfx, pZb),
		wavePipeline(gfx, pZb),
		texturePipeline(gfx, pZb),
		monke(ObjectLoader::LoadObjectNormal<BlendNormalVertex>(L"models/suzanne.obj")),
		lightSphere(Sphere::GetPlain<SolidColorVertex>(0.05f, 32, 32)),
		wavePlane(Plane::GetSkinnedNormal<TextureBlendNormalVertex>(2.0f, 30)),
		groundPlane(Plane::GetSkinnedNormal<TextureNormalVertex>(40.0f, 30)),
		wallPlane(Plane::GetSkinnedNormal<TextureNormalVertex>(40.0f, 30)),
		player(wnd.mouse, wnd.kbd)
	{
		Color c = Colors::White;

		pipeline.effect.ps.SetSurfaceColor(Colors::Green);
		pipeline.effect.ps.SetLightColor(c);

		wavePipeline.effect.ps.SetLightColor(c);
		wavePipeline.effect.ps.SetAmbient({ 0.15f, 0.15f, 0.15f });

		texturePipeline.effect.vs.SetAmbient({ 0.05f, 0.05f, 0.05f });
		texturePipeline.effect.vs.SetLightColor(Colors::Yellow);

		const Mat4 projection = Mat4::ProjectionFOV(fov, (float)Graphics::ScreenWidth/(float)Graphics::ScreenHeight, nearPlane, farPlane);
		pipeline.effect.vs.BindProjection(projection);
		solidColorPipeline.effect.vs.BindProjection(projection);
		wavePipeline.effect.vs.BindProjection(projection);
		texturePipeline.effect.vs.BindProjection(projection);

		wavePipeline.effect.ps.BindTexture(&waveTexture);

		player.SetMoveSpeed(playerMoveSpeed);
		player.SetSensitivity(camSensitity);

		for (auto i = lightSphere.vertices.begin(); i != lightSphere.vertices.end(); i++)
		{
			i->color = c;
		}
	}

	void UpdateModel(Keyboard& kbd, Mouse& mouse, float dt) override
	{
		player.UpdateCam(dt);

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

		time += dt;
		wavePipeline.effect.vs.SetTime(time);
	}

	void ComposeFrame() override
	{
		Mat4 camView = player.cam.GetViewTransform();
		pipeline.BeginFrame();

		pipeline.effect.vs.BindWorldTransformation(Mat4::Translation(monkePos));
		pipeline.effect.vs.BindView(camView);
		pipeline.effect.ps.SetLightPos(lightPos);
		pipeline.effect.ps.SetCamView(camView);
		pipeline.Draw(monke);

		solidColorPipeline.effect.vs.BindWorldTransformation(Mat4::Translation(lightPos));
		solidColorPipeline.effect.vs.BindView(camView);
		solidColorPipeline.Draw(lightSphere);
		
		wavePipeline.effect.vs.BindWorldTransformation(Mat4::Translation(wavePlanePos));
		wavePipeline.effect.vs.BindView(camView);
		wavePipeline.effect.ps.SetLightPos(lightPos);
		wavePipeline.effect.ps.SetCamView(camView);
		wavePipeline.Draw(wavePlane);

		texturePipeline.effect.vs.BindWorldTransformation(Mat4::Translation(groundPlanePos));
		texturePipeline.effect.vs.BindView(camView);
		texturePipeline.effect.vs.SetCamView(camView);
		texturePipeline.effect.vs.SetLightPos(player.GetPos());
		texturePipeline.effect.ps.BindTexture(&groundTexture);
		texturePipeline.Draw(groundPlane);

		texturePipeline.effect.ps.BindTexture(&wallTexture);
		for (int i = 0; i < 4; i++)
		{
			const Mat4 wallTransform = Mat4::RotationX(wallRot[i].x) * Mat4::RotationY(wallRot[i].y) * Mat4::RotationZ(wallRot[i].z) * Mat4::Translation(wallPos[i]);
			texturePipeline.effect.vs.BindWorldTransformation(wallTransform);
			texturePipeline.Draw(wallPlane);
		}
	}

private:
	Graphics& gfx;
	std::shared_ptr<ZBuffer> pZb;
	SpecularPipeline pipeline;
	SolidColorPipeline solidColorPipeline;
	WavePipeline wavePipeline;
	VertexLightPipeline texturePipeline;

	static constexpr float fov = 90.0f;
	static constexpr float nearPlane = 0.25f;
	static constexpr float farPlane = 100.0f;

	IndexedTriangleList<BlendNormalVertex> monke;
	IndexedTriangleList<SolidColorVertex> lightSphere;
	IndexedTriangleList<TextureBlendNormalVertex> wavePlane;
	IndexedTriangleList<TextureNormalVertex> groundPlane;
	IndexedTriangleList<TextureNormalVertex> wallPlane;
	Vec3 lightPos = { -1.0f, 1.0f, -16.0f };
	Vec3 monkePos = { 0.0f, 1.0f, -18.0f };
	Vec3 wavePlanePos = { -1.5f, 0.5f, -17.5f };
	Vec3 groundPlanePos = { 0.0f, 0.0f, 0.0f };
	float moveSpeed = 1.5f;

	Vec3 wallPos[4] = { {0.0f, -10.0f, -19.0f}, {0.0f, -10.0f, 19.0f}, {19.0f, -10.0f, 0.0f}, {-19.0f, -10.0f, 0.0f} };
	Vec3 wallRot[4] = { {0.0f, 0.0f, PI / 2}, {0.0f, 0.0f, -PI / 2}, {PI / 2, 0.0f, PI / 2}, {-PI / 2, 0.0f, PI / 2} };

	Surface waveTexture = Surface::FromFile(L"Images/eye.png");
	Surface groundTexture = Surface::FromFile(L"Images/groundTexture.png");
	Surface wallTexture = Surface::FromFile(L"Images/wallTexture.png");

	Player player;
	Vec2 camSensitity = { 0.00002f * fov, 0.00002f * fov };
	static constexpr float playerMoveSpeed = 10.0f;

	float time = 0.0f;
};