/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "ForestScene.h"
#include "SpecularScene.h"
#include "SpecularWaveScene.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	scenes.emplace_back(std::make_unique<ForestScene>(gfx, wnd, "Forest"));
	scenes.emplace_back(std::make_unique<SpecularScene>(gfx, "Specular lighting"));
	scenes.emplace_back(std::make_unique<SpecularWaveScene>(gfx, "Specular lighting 2"));
	
	curScene = scenes.begin();
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	lastDeltaTime = ft.Mark();

	while (!wnd.kbd.KeyIsEmpty())
	{
		const auto e = wnd.kbd.ReadKey();
		if (e.GetCode() == VK_TAB && e.IsPress())
		{
			curScene++;
			if (curScene == scenes.end()) curScene = scenes.begin();
		}
	}

	(*curScene)->UpdateModel(wnd.kbd, wnd.mouse, lastDeltaTime);
}

void Game::ComposeFrame()
{
	(*curScene)->ComposeFrame();

	int fps = (int)(1.0f / lastDeltaTime);
	gfx.DrawText("FPS: " + std::to_string(fps) + '\n' + (*curScene)->GetName() + '\n' + "Press Tab to change scene", 0, 0);
}