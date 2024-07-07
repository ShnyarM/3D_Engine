#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

class Scene
{
public:
	virtual void UpdateModel(Keyboard& kbd, Mouse& mouse, float dt) = 0;
	virtual void ComposeFrame() = 0;
	virtual ~Scene() = default;
};