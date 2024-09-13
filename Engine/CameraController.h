#pragma once
#include "Camera.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "MainWindow.h"

class CameraController
{
public:
	CameraController(MainWindow& wnd)
		:
		mouse(wnd.mouse),
		kbd(wnd.kbd)
	{}
	CameraController(Mouse& mouse, Keyboard& kbd)
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
	void UpdateCam(float dt = 1.0f)
	{
		Mat4 camRot = cam.GetRotTransform();
		//Camera Movement
		if (kbd.KeyIsPressed('W'))
		{
			cam.AddToPos(Vec4{ 0.0f, 0.0f, moveSpeed * dt } * camRot);
		}
		if (kbd.KeyIsPressed('S')) 
		{
			cam.AddToPos(Vec4{ 0.0f, 0.0f, -moveSpeed * dt } * camRot);
		}
		if (kbd.KeyIsPressed('D'))
		{
			cam.AddToPos(Vec4{ moveSpeed * dt, 0.0f, 0.0f } * camRot);
		}
		if (kbd.KeyIsPressed('A'))
		{
			cam.AddToPos(Vec4{ -moveSpeed * dt, 0.0f, 0.0f } *camRot);
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
					cam.AddToRot({ 0.0f, (float)mouseDelta.x * sensitivity.x, (float)mouseDelta.y * sensitivity.y });
					cam.ClampZ();
					mousePos = newMousePos;
				}
				break;
			}
		}
	}
public:
	Camera cam;
private:
	Mouse& mouse;
	Keyboard& kbd;

	float moveSpeed = 1.5f;
	Vec2 sensitivity = { 0.005f, 0.005f };

	Vei2 mousePos = { 0, 0 };
	bool mouseIsPressed = false;
};
