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
//#include "TextureCubeScene.h"
//#include "VertexColorScene.h"
//#include "TextureShadingScene.h"
//#include "TwoCubesScene.h"
//#include "WaveScene.h"
//#include "FlatShadingScene.h"
//#include "ShadingScene.h"
//#include "SphereScene.h"
//#include "PointLightScene.h"
//#include "PointLightScenePlane.h"
#include "SpecularScene.h"
#include "PlaneScene.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	scenes.emplace_back(std::make_unique<SpecularScene>(gfx, wnd, "Forest"));
	scenes.emplace_back(std::make_unique<PlaneScene>(gfx, "Specular lighting", L"models\\suzanne.obj"));
	/*scenes.emplace_back(std::make_unique<PointLightScenePlane>(gfx, "Pixel light with plane", L"models\\suzanne.obj"));
	scenes.emplace_back(std::make_unique<PointLightScene>(gfx, "Pixel light with calculated normals", L"models\\bunny.obj"));
	scenes.emplace_back(std::make_unique<SphereScene>(gfx, "Directional Pixel light", L"models\\suzanne.obj"));
	scenes.emplace_back(std::make_unique<ShadingScene>(gfx, "Gouraud Shading", L"models\\suzanne.obj"));
	scenes.emplace_back(std::make_unique<FlatShadingScene>(gfx, "Flat Shading", L"models\\suzanne.obj"));
	scenes.emplace_back(std::make_unique<WaveScene>(gfx, "Wave effect with Gouraud", L"Images\\eye.png"));
	scenes.emplace_back(std::make_unique<TwoCubesScene>(gfx, "Zbuffer"));
	scenes.emplace_back(std::make_unique<TextureCubeScene>(gfx, "Texture", L"Images\\dice_skin.png"));
	scenes.emplace_back(std::make_unique<TextureShadingScene>(gfx, "Load files with textures and normals", L"Images\\default.png"));*/
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
	gfx.DrawText("FPS: " + std::to_string(fps) + '\n' + (*curScene)->GetName(), 0, 0);
}