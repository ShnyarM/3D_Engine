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

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
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
	const float dt = 1.0f / 60.0f;
	if (wnd.kbd.KeyIsPressed('Q')) //up
	{
		theta_z += dTheta * dt;
	}
	if (wnd.kbd.KeyIsPressed('W')) //right
	{
		theta_y += dTheta * dt;
	}
	if (wnd.kbd.KeyIsPressed('E')) //left
	{
		theta_z += dTheta * dt;
	}

	if (wnd.kbd.KeyIsPressed('D'))
	{
		cubeOffset.x += moveSpeed;
	}
	if (wnd.kbd.KeyIsPressed('A'))
	{
		cubeOffset.x -= moveSpeed;
	}

	if (wnd.kbd.KeyIsPressed('R'))
	{
		cubeOffset.z += moveSpeed;
	}
	if (wnd.kbd.KeyIsPressed('F'))
	{
		cubeOffset.z -= moveSpeed;
	}

	if (wnd.kbd.KeyIsPressed('Z'))
	{
		spaceTransformer.screenDistance += moveSpeed;
	}
	if (wnd.kbd.KeyIsPressed('H'))
	{
		spaceTransformer.screenDistance -= moveSpeed;
	}
}
#include "Mat3.h"
void Game::ComposeFrame()
{
	/*const Color colors[12] = {
		Colors::White,
		Colors::Blue,
		Colors::Cyan,
		Colors::Gray,
		Colors::Green,
		Colors::Magenta,
		Colors::LightGray,
		Colors::Red,
		Colors::Yellow,
		Colors::White,
		Colors::Blue,
		Colors::Cyan
	};*/

	const Color colors[12] = {
		Colors::White,
		Colors::Blue,
		Colors::Cyan,
		Colors::Gray,
		Colors::Green,
		Colors::Magenta,
	};

	Mat3 rot = Mat3::RotationX(theta_x) * Mat3::RotationY(theta_y) * Mat3::RotationZ(theta_z);
	auto triangles = cube.GetTriangles();

	for (auto& v : triangles.vertices)
	{
		v *= rot;
		v += cubeOffset;
	}

	for (size_t i = 0, end = triangles.indices.size()/3; i < end; i++)
	{
		const Vec3& v0 = triangles.vertices[triangles.indices[3*i]];
		const Vec3& v1 = triangles.vertices[triangles.indices[3*i+1]];
		const Vec3& v2 = triangles.vertices[triangles.indices[3*i+2]];
		Vec3 normal = (v1 - v0) % (v2 - v0);
		triangles.cullFlags[i] = normal * v0 >= 0;
	}

	for (auto& v : triangles.vertices)
	{
		spaceTransformer.Transform(v);
	}

	for (size_t i = 0, end = triangles.indices.size() / 3; i < end; i++)
	{
		if (triangles.cullFlags[i]) continue;
		gfx.DrawTriangle(triangles.vertices[triangles.indices[i * 3]],
			triangles.vertices[triangles.indices[i * 3 + 1]],
			triangles.vertices[triangles.indices[i * 3 + 2]],
			colors[i/2]);
	}


	/*auto lines = cube.GetLines();

	for (auto& v : lines.vertices)
	{

		v *= rot;
		v += cubeOffset;

		spaceTransformer.Transform(v);
	}

	for (auto i = lines.indices.cbegin(), end = lines.indices.cend(); i != end; std::advance(i, 2))
	{
		gfx.DrawLine(lines.vertices[*i], lines.vertices[*std::next(i)], Colors::Black);
	}*/
}