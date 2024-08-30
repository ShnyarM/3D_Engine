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
#include "TextureCubeScene.h"
#include "VertexColorScene.h"
#include "SolidColorScene.h"
#include "TwoCubesScene.h"
#include "WaveScene.h"
#include "PopOutScene.h"
#include "ShadingScene.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	scenes.emplace_back(std::make_unique<ShadingScene>(gfx, L"models\\suzanne.obj"));
	scenes.emplace_back(std::make_unique<WaveScene>(gfx, L"Images\\sauron-bhole.png"));
	scenes.emplace_back(std::make_unique<PopOutScene>(gfx, L"Images\\dice_skin.png"));
	scenes.emplace_back(std::make_unique<TwoCubesScene>(gfx));
	scenes.emplace_back(std::make_unique<TextureCubeScene>(gfx, L"Images\\dice_skin.png"));
	scenes.emplace_back(std::make_unique<VertexColorScene>(gfx));
	scenes.emplace_back(std::make_unique<SolidColorScene>(gfx, L"Images\\default.png"));
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
	const float dt = ft.Mark();

	while (!wnd.kbd.KeyIsEmpty())
	{
		const auto e = wnd.kbd.ReadKey();
		if (e.GetCode() == VK_TAB && e.IsPress())
		{
			curScene++;
			if (curScene == scenes.end()) curScene = scenes.begin();
		}
	}

	(*curScene)->UpdateModel(wnd.kbd, wnd.mouse, dt);
}

void Game::ComposeFrame()
{
	(*curScene)->ComposeFrame();
}