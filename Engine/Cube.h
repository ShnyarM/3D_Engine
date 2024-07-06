#pragma once
#include <vector>
#include "Vec3.h"
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"
#include "TexVertex.h"

class Cube
{
public:
	Cube(float size)
	{
		float side = 0.5f * size;
		vertices.emplace_back(-size, size, -size);
		vertices.emplace_back(size, size, -size);
		vertices.emplace_back(size, -size, -size);
		vertices.emplace_back(-size, -size, -size);
		vertices.emplace_back(-size, -size, size);
		vertices.emplace_back(size, -size, size);
		vertices.emplace_back(size, size, size);
		vertices.emplace_back(-size, size, size);
	}

	IndexedLineList GetLines() const
	{
		return IndexedLineList{
			vertices,
			{0,1, 0,3, 0,7, 5,2, 5,4, 5,6, 6,1, 6,7, 4,7, 4,3, 2,1, 2,3}
		};
	}

	IndexedTriangleList<Vec3> GetTriangles() const
	{
		return IndexedTriangleList<Vec3>{
			vertices,
			{4,5,7, 6,7,5, 
			 4,7,3, 0,3,7, 
			 1,2,0, 3,0,2, 
			 6,5,2, 1,6,2, 
			 7,6,0, 1,0,6,
			 4,3,5, 2,5,3}
		};
	}

	IndexedTriangleList<TexVertex> GetTrianglesTex() const
	{
		std::vector<TexVertex> texVertices;

		//move vertices into textVertex
		for (auto& vertex : vertices)
		{
			texVertices.emplace_back(vertex, Vec2{ 0.0f, 0.0f });
		}

		//Set uv coordinates for each vertex
		texVertices[0].tPos = { 0.0f, 0.0f };
		texVertices[1].tPos = { 1.0f, 0.0f };
		texVertices[2].tPos = { 1.0f, 1.0f };
		texVertices[3].tPos = { 0.0f, 1.0f };
		texVertices[4].tPos = { 1.0f, 1.0f };
		texVertices[5].tPos = { 0.0f, 1.0f };
		texVertices[6].tPos = { 0.0f, 0.0f };
		texVertices[7].tPos = { 1.0f, 0.0f };

		return IndexedTriangleList<TexVertex>{
			std::move(texVertices),
			{ 4,5,7, 6,7,5,
			 4,7,3, 0,3,7,
			 1,2,0, 3,0,2,
			 6,5,2, 1,6,2,
			 7,6,0, 1,0,6,
			 4,3,5, 2,5,3 }
		};
	}

private:
	std::vector<Vec3> vertices;
};