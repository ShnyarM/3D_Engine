#pragma once
#include "Camera.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "MainWindow.h"

class Player
{
public:
	Player(MainWindow& wnd)
		:
		mouse(wnd.mouse),
		kbd(wnd.kbd)
	{}
	Player(Mouse& mouse, Keyboard& kbd)
		:
		mouse(mouse),
		kbd(kbd),
		mousePos(mouse.GetPosX(), mouse.GetPosY())
	{}
	void SetMoveSpeed(float speed)
	{
		moveSpeed = speed;
	}
	float GetSpeed() const
	{
		return moveSpeed;
	}
	void SetSensitivity(const Vec2& newSens)
	{
		sensitivity = newSens;
	}
	Vec3 GetPos() const
	{
		return pos;
	}
	void UpdateCam(float dt = 1.0f)
	{
		Mat4 camRot = Mat4::RotationY(cam.GetRot().y);

		//Camera Movement
		if (kbd.KeyIsPressed('W'))
		{
			pos += Vec4{ 0.0f, 0.0f, moveSpeed * dt } * camRot;
		}
		if (kbd.KeyIsPressed('S')) 
		{
			pos += Vec4{ 0.0f, 0.0f, -moveSpeed * dt } * camRot;
		}
		if (kbd.KeyIsPressed('D'))
		{
			pos += Vec4{ moveSpeed * dt, 0.0f, 0.0f } * camRot;
		}
		if (kbd.KeyIsPressed('A'))
		{
			pos += Vec4{ -moveSpeed * dt, 0.0f, 0.0f } * camRot;
		}

		while (!mouse.IsEmpty())
		{
			const Mouse::Event e = mouse.Read();
			switch (e.GetType())
			{
			case Mouse::Event::Type::LPress:
				mousePos = e.GetPos();
				mouseIsPressed = true;
				break;
			case Mouse::Event::Type::LRelease:
				mouseIsPressed = false;
				break;
			case Mouse::Event::Type::Move:
				if (mouseIsPressed)
				{
					Vei2 newMousePos = e.GetPos();
					Vei2 mouseDelta = newMousePos - mousePos;
					rot += { 0.0f, (float)mouseDelta.x * sensitivity.x, (float)mouseDelta.y * sensitivity.y };
					rot.z = std::max(-PI / 3.0f, std::min(PI / 3.0f, rot.z));
					mousePos = newMousePos;
				}
				break;
			}
		}

		cam.SetPos({ pos.x, playerHeight, pos.z });
		cam.SetRot(rot);
	}
public:
	Camera cam;
private:
	Mouse& mouse;
	Keyboard& kbd;

	static constexpr float playerHeight = 1.0f;
	Vec3 pos = { 0.0f, playerHeight / 2.0f, 0.0f };
	Vec3 rot = { 0.0f, 0.0f, 0.0f };

	float moveSpeed = 1.5f;
	Vec2 sensitivity = { 0.005f, 0.005f };

	Vei2 mousePos = { 0, 0 };
	bool mouseIsPressed = false;
};
