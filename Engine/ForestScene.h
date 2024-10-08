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
#include "TextureShader.h"

class ForestScene : public Scene
{
	// rename pipelines
	//typedef Pipeline<SpecularPixelPointEffect> SpecularPipeline;
	typedef Pipeline<SolidColorEffect> SolidColorPipeline;
	//typedef Pipeline<SpecularTexturedWaveEffect> WavePipeline;
	typedef Pipeline<TexturedVertexPointEffect> VertexLightPipeline;
	typedef Pipeline<TextureShader> TexturePipeline;

public:
	ForestScene(Graphics& gfx, MainWindow& wnd, const char* name)
		:
		Scene(name),
		gfx(gfx),
		pZb(std::make_unique<ZBuffer>(gfx.ScreenWidth, gfx.ScreenHeight)),
		//pipeline(gfx, pZb),
		solidColorPipeline(gfx, pZb),
		//wavePipeline(gfx, pZb),
		textureLightPipeline(gfx, pZb),
		texturePipeline(gfx, pZb),
		//monke(ObjectLoader::LoadObjectNormal<BlendNormalVertex>(L"models/suzanne.obj")),
		//lightSphere(Sphere::GetPlain<SolidColorVertex>(0.05f, 32, 32)),
		//wavePlane(Plane::GetSkinnedNormal<TextureBlendNormalVertex>(2.0f, 60)),
		groundPlane(Plane::GetSkinnedNormal<TextureNormalVertex>(40.0f, 30)),
		wallPlane(Plane::GetSkinnedNormal<TextureNormalVertex>(40.0f, 30)),
		torchBase(ObjectLoader::LoadObjectNormalTextured<TextureNormalVertex>(L"models/torch.obj", 0.0222f, 1)),
		torchFlame(ObjectLoader::LoadObject<SolidColorVertex>(L"models/torch.obj", 0.0222f, 2)),
		tree(ObjectLoader::LoadObjectNormalTextured<TextureNormalVertex>(L"models/tree.obj", 2.0f)),
		//torchFlame(Sphere::GetPlain<SolidColorVertex>(0.05f, 32, 32)),
		player(wnd.mouse, wnd.kbd)
	{
		Color flameColor{ 255, 132, 0 };

		//pipeline.effect.ps.SetSurfaceColor(Colors::Green);
		//pipeline.effect.ps.SetLightColor(c);

		//wavePipeline.effect.ps.SetLightColor(c);
		//wavePipeline.effect.ps.SetAmbient({ 0.15f, 0.15f, 0.15f });

		textureLightPipeline.effect.vs.SetAmbient({ 0.05f, 0.05f, 0.05f });
		textureLightPipeline.effect.vs.SetLightColor(flameColor);

		const Mat4 projection = Mat4::ProjectionFOV(fov, (float)Graphics::ScreenWidth/(float)Graphics::ScreenHeight, nearPlane, farPlane);
		//pipeline.effect.vs.BindProjection(projection);
		solidColorPipeline.effect.vs.BindProjection(projection);
		//wavePipeline.effect.vs.BindProjection(projection);
		textureLightPipeline.effect.vs.BindProjection(projection);
		texturePipeline.effect.vs.BindProjection(projection);

		//wavePipeline.effect.ps.BindTexture(&waveTexture);

		player.SetMoveSpeed(playerMoveSpeed);
		player.SetSensitivity(camSensitity);

		for (auto i = torchFlame.vertices.begin(); i != torchFlame.vertices.end(); i++)
		{
			i->color = flameColor;
		}
	}

	void UpdateModel(Keyboard& kbd, Mouse& mouse, float dt) override
	{
		player.Update(dt);

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
		//wavePipeline.effect.vs.SetTime(time);

		//torch position
		Mat4 camRotTransform = player.cam.GetRotTransform();
		Vei2 mouseVelocity = player.GetMouseVelocity();

		Vec3 torchOffset = Vec3{ 0.3f, -0.3f, 0.5f } + Vec3{ -mouseVelocity.x * 0.001f, mouseVelocity.y * 0.001f, 0.0f };
		Vec3 torchFlameOffset = Vec3{ 0.3f, 0.1f, 0.6f } + Vec3{ -mouseVelocity.x * 0.001f, mouseVelocity.y * 0.001f, 0.0f };

		torchPos = player.GetPos() + (Vec4(torchOffset) * camRotTransform);
		torchFlamePos = player.GetPos() + (Vec4(torchFlameOffset) * camRotTransform);
	}

	void ComposeFrame() override
	{
		Mat4 camView = player.cam.GetViewTransform();
		textureLightPipeline.BeginFrame();

		textureLightPipeline.effect.vs.BindCompleteView(camView);
		textureLightPipeline.effect.vs.SetLightPos(torchFlamePos);
		solidColorPipeline.effect.vs.BindView(camView);

		//pipeline.effect.vs.BindWorldTransformation(Mat4::Translation(monkePos));
		//pipeline.effect.vs.BindView(camView);
		//pipeline.effect.ps.SetLightPos(lightPos);
		//pipeline.effect.ps.SetCamView(camView);
		//pipeline.Draw(monke);

		//solidColorPipeline.effect.vs.BindWorldTransformation(Mat4::Translation(lightPos));
		//solidColorPipeline.effect.vs.BindView(camView);
		//solidColorPipeline.Draw(lightSphere);
		
		//wavePipeline.effect.vs.BindWorldTransformation(Mat4::Translation(wavePlanePos));
		//wavePipeline.effect.vs.BindView(camView);
		//wavePipeline.effect.ps.SetLightPos(lightPos);
		//wavePipeline.effect.ps.SetCamView(camView);
		//wavePipeline.Draw(wavePlane);

		//Vec3 camRot = player.cam.GetRot();
		//Vei2 mouseVelocity = player.GetMouseVelocity();

		//Vec3 torchOffset = Vec3{ 0.3f, -0.3f, 0.5f } + Vec3{ -mouseVelocity.x * 0.001f, mouseVelocity.y * 0.001f, 0.0f };
		//Vec3 torchPos = player.GetPos() + (Vec4(torchOffset) * player.cam.GetRotTransform());
		//Mat4 torchTransform = Mat4::RotationZ(camRot.z + (PI / 7)) * Mat4::RotationY(camRot.y) * Mat4::Translation(torchPos);

		textureLightPipeline.effect.vs.BindWorldTransformation(Mat4::Translation(groundPlanePos));
		textureLightPipeline.effect.ps.BindTexture(&groundTexture);
		textureLightPipeline.Draw(groundPlane);

		textureLightPipeline.effect.ps.BindTexture(&wallTexture);
		for (int i = 0; i < 4; i++)
		{
			const Mat4 wallTransform = Mat4::RotationX(wallRot[i].x) * Mat4::RotationY(wallRot[i].y) * Mat4::RotationZ(wallRot[i].z) * Mat4::Translation(wallPos[i]);
			textureLightPipeline.effect.vs.BindWorldTransformation(wallTransform);
			textureLightPipeline.Draw(wallPlane);
		}

		//Vec3 camRot = player.cam.GetRot();
		//Vei2 mouseVelocity = player.GetMouseVelocity();

		//Vec3 torchOffset = Vec3{ 0.3f, -0.3f, 0.5f } + Vec3{ -mouseVelocity.x*0.001f, mouseVelocity.y*0.001f, 0.0f };
		//Vec3 torchPos = player.GetPos() + (Vec4(torchOffset) * player.cam.GetRotTransform());
		//Mat4 torchTransform = Mat4::RotationZ(camRot.z + (PI/7)) * Mat4::RotationY(camRot.y) * Mat4::Translation(torchPos);

		Vec3 camRot = player.cam.GetRot();
		Mat4 torchTransform = Mat4::RotationZ(camRot.z + (PI / 7)) * Mat4::RotationY(camRot.y) * Mat4::Translation(torchPos);
		Mat4 flametransform = Mat4::RotationZ(camRot.z + (PI / 7)) * Mat4::RotationY(camRot.y) * Mat4::Translation(torchFlamePos);

		textureLightPipeline.effect.vs.BindWorldTransformation(torchTransform);
		textureLightPipeline.effect.ps.BindTexture(&torchBaseTexture);
		textureLightPipeline.Draw(torchBase);

		textureLightPipeline.effect.vs.BindWorldTransformation(Mat4::Identity());
		textureLightPipeline.effect.ps.BindTexture(&treeTexture);
		for (int i = 0; i < 6; i++)
		{
			textureLightPipeline.effect.vs.BindWorldTransformation(Mat4::Translation(treePos[i]));
			textureLightPipeline.Draw(tree);
		}

		solidColorPipeline.effect.vs.BindWorldTransformation(torchTransform);
		solidColorPipeline.Draw(torchFlame);
	}

private:
	Graphics& gfx;
	std::shared_ptr<ZBuffer> pZb;
	//SpecularPipeline pipeline;
	SolidColorPipeline solidColorPipeline;
	//WavePipeline wavePipeline;
	VertexLightPipeline textureLightPipeline;
	TexturePipeline texturePipeline;

	static constexpr float fov = 90.0f;
	static constexpr float nearPlane = 0.25f;
	static constexpr float farPlane = 100.0f;

	//IndexedTriangleList<BlendNormalVertex> monke;
	//IndexedTriangleList<SolidColorVertex> lightSphere;
	//IndexedTriangleList<TextureBlendNormalVertex> wavePlane;
	IndexedTriangleList<TextureNormalVertex> groundPlane;
	IndexedTriangleList<TextureNormalVertex> wallPlane;
	IndexedTriangleList<TextureNormalVertex> torchBase;
	IndexedTriangleList<SolidColorVertex> torchFlame;
	IndexedTriangleList<TextureNormalVertex> tree;
	Vec3 lightPos = { -1.0f, 1.0f, -16.0f };
	Vec3 monkePos = { 0.0f, 1.0f, -18.0f };
	Vec3 wavePlanePos = { -1.5f, 0.5f, -17.5f };
	Vec3 groundPlanePos = { 0.0f, 0.0f, 0.0f };
	Vec3 torchPos = { 0.0f, 0.0f, 0.0f };
	Vec3 torchFlamePos = { 0.0f, 0.0f, 0.0f };
	float moveSpeed = 1.5f;

	Vec3 wallPos[4] = { {0.0f, -10.0f, -19.0f}, {0.0f, -10.0f, 19.0f}, {19.0f, -10.0f, 0.0f}, {-19.0f, -10.0f, 0.0f} };
	Vec3 wallRot[4] = { {0.0f, 0.0f, PI / 2}, {0.0f, 0.0f, -PI / 2}, {PI / 2, 0.0f, PI / 2}, {-PI / 2, 0.0f, PI / 2} };
	Vec3 treePos[6] = { {-14.0f, -1.0f, 16.0f}, {4.0f, -1.0f, 11.0f},{11.0f, -1.0f, -13.0f},{8.0f, -1.0f, 2.0f},{-10.0f, -1.0f, -12.0f},{-10.0f, -1.0f, -2.0f} };

	Surface waveTexture = Surface::FromFile(L"Images/eye.png");
	Surface groundTexture = Surface::FromFile(L"Images/groundTexture.png");
	Surface wallTexture = Surface::FromFile(L"Images/wallTexture.png");
	Surface torchBaseTexture = Surface::FromFile(L"Images/torchTexture.png");
	Surface treeTexture = Surface::FromFile(L"Images/treeTexture.png");

	Player player;
	Vec2 camSensitity = { 0.00002f * fov, 0.00002f * fov };
	static constexpr float playerMoveSpeed = 5.0f;

	float time = 0.0f;
};