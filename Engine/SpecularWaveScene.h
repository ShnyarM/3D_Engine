#pragma once
#include "Scene.h"
#include "Sphere.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Mat.h"
#include "ChiliMath.h"
#include "Pipeline.h"
#include "TextureBlendNormalVertex.h"
#include "SolidColorEffect.h"
#include "ObjectLoader.h"
#include "Plane.h"

class SpecularWaveScene : public Scene
{
public:
	SpecularWaveScene(Graphics& gfx, const char* name)
		:
		Scene(name),
		gfx(gfx),
		pZb(std::make_unique<ZBuffer>(gfx.ScreenWidth, gfx.ScreenHeight)),
		solidColorPipeline(gfx, pZb),
		wavePipeline(gfx, pZb),
		lightSphere(Sphere::GetPlain<SolidColorVertex>(0.05f, 32, 32)),
		wave(Plane::GetSkinnedNormal<TextureBlendNormalVertex>(1.0f, 60.0f))
	{
		Color c = Colors::White;
		wavePipeline.effect.ps.SetLightColor(c);
		wavePipeline.effect.ps.SetAmbient({ 0.2f, 0.2f, 0.2f });

		const Mat4 projection = Mat4::ProjectionFOV(fov, (float)Graphics::ScreenWidth / (float)Graphics::ScreenHeight, nearPlane, farPlane);
		solidColorPipeline.effect.vs.BindProjection(projection);
		wavePipeline.effect.vs.BindProjection(projection);

		for (auto i = lightSphere.vertices.begin(); i != lightSphere.vertices.end(); i++)
		{
			i->color = c;
		}

		wavePipeline.effect.ps.BindTexture(&waveTexture);
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
			wavePos.x += moveSpeed * dt;
		}
		if (kbd.KeyIsPressed('J'))
		{
			wavePos.x -= moveSpeed * dt;
		}
		if (kbd.KeyIsPressed('I'))
		{
			wavePos.z += moveSpeed * dt;
		}
		if (kbd.KeyIsPressed('K'))
		{
			wavePos.z -= moveSpeed * dt;
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

		time += dt;
		wavePipeline.effect.vs.SetTime(time);
	}

	void ComposeFrame() override
	{
		wavePipeline.BeginFrame();

		Mat4 waveTransform = Mat4::RotationX(theta_x) * Mat4::RotationY(theta_y) * Mat4::RotationZ(theta_z) * Mat4::Translation(wavePos);
		wavePipeline.effect.vs.BindWorldTransformation(waveTransform);
		wavePipeline.effect.ps.SetLightPos(lightPos);
		wavePipeline.Draw(wave);

		solidColorPipeline.effect.vs.BindWorldTransformation(Mat4::Translation(lightPos));
		solidColorPipeline.Draw(lightSphere);

		gfx.DrawText("Use (W,A,S,D), (T,F,G,H) and (I,J,K,L) to move the objects", 0, Graphics::ScreenHeight - gfx.GetFont().GetCHeight());
	}

private:
	Graphics& gfx;
	std::shared_ptr<ZBuffer> pZb;
	Pipeline<SolidColorEffect> solidColorPipeline;
	Pipeline<SpecularTexturedWaveEffect> wavePipeline;

	static constexpr float fov = 90.0f;
	static constexpr float nearPlane = 0.25f;
	static constexpr float farPlane = 10.0f;

	static constexpr float dTheta = 1.4f;
	float theta_x = 0.0f;
	float theta_y = 0.0f;
	float theta_z = 0.0f;
	float time = 0.0f;

	IndexedTriangleList<SolidColorVertex> lightSphere;
	IndexedTriangleList<TextureBlendNormalVertex> wave;
	Vec3 lightPos = { 0.0f, 0.0f, 2.0f };
	Vec3 wavePos = { 0.0f, 0.0f, 2.0f };
	static constexpr float moveSpeed = 1.5f;

	Surface waveTexture = Surface::FromFile(L"Images/eye.png");
};