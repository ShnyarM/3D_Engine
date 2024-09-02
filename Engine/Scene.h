#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

class Scene
{
protected:
	Scene()
		:
		name("Nameless Scene")
	{}

	Scene(const char* name)
		:
		name(name)
	{}
public:
	virtual void UpdateModel(Keyboard& kbd, Mouse& mouse, float dt) = 0;
	virtual void ComposeFrame() = 0;
	virtual ~Scene() = default;

	const char* GetName() const
	{
		return name;
	}

protected:
	const char* name;
};