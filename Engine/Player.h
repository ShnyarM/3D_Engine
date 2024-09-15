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
	Vei2 GetMouseVelocity() const
	{
		return mouseVelocity;
	}
	Vec3 GetVelocity() const
	{
		return velocity;
	}
	Vec3 GetPos() const
	{
		return pos;
	}
	void Update(float dt = 1.0f)
	{
		Mat4 camRot = Mat4::RotationY(cam.GetRot().y);
		Vec3 newVelocity = { 0.0f, velocity.y, 0.0f };

		//Movement
		if (kbd.KeyIsPressed('W'))
		{
			newVelocity += Vec4{ 0.0f, 0.0f, moveSpeed * dt } * camRot;
		}
		if (kbd.KeyIsPressed('S')) 
		{
			newVelocity += Vec4{ 0.0f, 0.0f, -moveSpeed * dt } * camRot;
		}
		if (kbd.KeyIsPressed('D'))
		{
			newVelocity += Vec4{ moveSpeed * dt, 0.0f, 0.0f } * camRot;
		}
		if (kbd.KeyIsPressed('A'))
		{
			newVelocity += Vec4{ -moveSpeed * dt, 0.0f, 0.0f } * camRot;
		}

		//Jumping
		if (pos.y <= playerHeight / 2.0f && !onGround)
		{
			onGround = true;
			pos.y = playerHeight / 2.0f;
			newVelocity.y = 0.0f;
		}

		if (!onGround)
		{
			newVelocity.y = velocity.y - gravity * dt;
		}

		if (kbd.KeyIsPressed(' ') && onGround)
		{
			onGround = false;
			newVelocity.y = 0.075f;
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
					mouseVelocity = newMousePos - mousePos;
					rot += { 0.0f, (float)mouseVelocity.x * sensitivity.x, (float)mouseVelocity.y * sensitivity.y };
					rot.z = std::max(-PI / 3.0f, std::min(PI / 3.0f, rot.z));
					mousePos = newMousePos;
				}
				break;
			}
		}

		velocity = newVelocity;
		pos += velocity;

		if (pos.x > 18.5f) pos.x = 18.5f;
		if (pos.x < -18.5f) pos.x = -18.5f;
		if (pos.z > 18.5f) pos.z = 18.5f;
		if (pos.z < -18.5f) pos.z = -18.5f;

		cam.SetPos(pos);
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
	Vec3 velocity = { 0.0f, 0.0f, 0.0f };

	float moveSpeed = 1.5f;
	Vec2 sensitivity = { 0.005f, 0.005f };
	bool onGround = true;
	float gravity = 0.3f;

	Vei2 mousePos = { 0, 0 };
	Vei2 mouseVelocity = { 0, 0 };
	bool mouseIsPressed = false;
};
