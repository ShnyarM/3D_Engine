#pragma once

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
			{ 4,5,7, 6,7,5,
			 4,7,3, 0,3,7,
			 1,2,0, 3,0,2,
			 6,5,2, 1,6,2,
			 7,6,0, 1,0,6,
			 4,3,5, 2,5,3 }
		};
	}

	IndexedTriangleList<TexVertex> GetTrianglesTex() const
	{
		std::vector<TexVertex> texVertices;
		texVertices.reserve(vertices.size());

		//move vertices into textVertex
		for (auto& vertex : vertices)
		{
			texVertices.emplace_back(vertex, Vec2{ 0.0f, 0.0f });
		}

		//Set uv coordinates for each vertex
		texVertices[0].tPos = { 0.0f, 0.0f };
		texVertices[1].tPos = { 2.0f, 0.0f };
		texVertices[2].tPos = { 2.0f, 2.0f };
		texVertices[3].tPos = { 0.0f, 2.0f };
		texVertices[4].tPos = { 2.0f, 2.0f };
		texVertices[5].tPos = { 0.0f, 2.0f };
		texVertices[6].tPos = { 0.0f, 0.0f };
		texVertices[7].tPos = { 2.0f, 0.0f };

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

class FoldedCube
{
public:
	FoldedCube(float size)
	{
		float side = 0.5f * size;
		vertices.reserve(14);
		vertices.emplace_back(-size, size, -size);	// 0
		vertices.emplace_back(size, size, -size);	// 1
		vertices.emplace_back(-size, -size, -size);	// 2
		vertices.emplace_back(size, -size, -size);	// 3
		vertices.emplace_back(-size, -size, size);	// 4
		vertices.emplace_back(size, -size, size);	// 5
		vertices.emplace_back(-size, size, size);	// 6
		vertices.emplace_back(size, size, size);	// 7
		vertices.emplace_back(-size, size, size);	// 8
		vertices.emplace_back(-size, size, -size);	// 9
		vertices.emplace_back(size, size, size);	// 10
		vertices.emplace_back(size, size, -size);	// 11
		vertices.emplace_back(-size, size, size);	// 12
		vertices.emplace_back(-size, size, -size);	// 13

		tCords.reserve(14);
		//tCords.emplace_back(0.0f, 0.0f); // 0
		//tCords.emplace_back(1.0f, 0.0f); // 1
		//tCords.emplace_back(0.0f, 1.0f); // 2
		//tCords.emplace_back(1.0f, 1.0f); // 3
		//tCords.emplace_back(0.0f, 0.0f); // 4
		//tCords.emplace_back(1.0f, 0.0f); // 5
		//tCords.emplace_back(0.0f, 1.0f); // 6
		//tCords.emplace_back(1.0f, 1.0f); // 7
		//tCords.emplace_back(1.0f, 0.0f); // 8
		//tCords.emplace_back(1.0f, 1.0f); // 9
		//tCords.emplace_back(0.0f, 0.0f); // 10
		//tCords.emplace_back(0.0f, 1.0f); // 11
		//tCords.emplace_back(1.0f, 0.0f); // 12
		//tCords.emplace_back(1.0f, 1.0f); // 13

		//this one wraps
		tCords.emplace_back(0.0f, 0.0f); // 0
		tCords.emplace_back(2.0f, 0.0f); // 1
		tCords.emplace_back(0.0f, 2.0f); // 2
		tCords.emplace_back(2.0f, 2.0f); // 3
		tCords.emplace_back(0.0f, 0.0f); // 4
		tCords.emplace_back(2.0f, 0.0f); // 5
		tCords.emplace_back(0.0f, 2.0f); // 6
		tCords.emplace_back(2.0f, 2.0f); // 7
		tCords.emplace_back(2.0f, 0.0f); // 8
		tCords.emplace_back(2.0f, 2.0f); // 9
		tCords.emplace_back(0.0f, 0.0f); // 10
		tCords.emplace_back(0.0f, 2.0f); // 11
		tCords.emplace_back(2.0f, 0.0f); // 12
		tCords.emplace_back(2.0f, 2.0f); // 13
	}

	IndexedLineList GetLines() const
	{
		return IndexedLineList{
			vertices,
			{0,1,  1,3,
			 2,3,  2,0,
			 6,7,  7,5,
			 5,4,  4,6,
			 0,6,  2,4,
			 1,7,  3,5}
		};
	}

	IndexedTriangleList<Vec3> GetTriangles() const
	{
		return IndexedTriangleList<Vec3>{
			vertices,
			{ 2,0,3, 1,3,0,
			 2,3,4, 5,4,3,
			 4,5,6, 7,6,5,
			 4,8,2, 9,2,8,
			 3,11,5, 10,5,11,
			 11,13,10, 12,10,13 }
		};
	}

	IndexedTriangleList<TexVertex> GetTrianglesTex() const
	{
		std::vector<TexVertex> texVertices;
		texVertices.reserve(vertices.size());

		for (int i = 0; i < vertices.size(); i++)
		{
			texVertices.emplace_back(vertices[i], tCords[i]);
		}

		return IndexedTriangleList<TexVertex>{
			std::move(texVertices),
			{ 2,0,3, 1,3,0,
			 2,3,4, 5,4,3,
			 4,5,6, 7,6,5,
			 4,8,2, 9,2,8,
			 3,11,5, 10,5,11,
			 11,13,10, 12,10,13 }
		};
	}

private:
	std::vector<Vec3> vertices;
	std::vector<Vec2> tCords;
};

class SkinnedCube
{
public:
	SkinnedCube(float size)
	{
		float side = 0.5f * size;
		vertices.reserve(14);
		vertices.emplace_back(-size, size, -size);	// 0
		vertices.emplace_back(size, size, -size);	// 1
		vertices.emplace_back(-size, -size, -size);	// 2
		vertices.emplace_back(size, -size, -size);	// 3
		vertices.emplace_back(-size, -size, size);	// 4
		vertices.emplace_back(size, -size, size);	// 5
		vertices.emplace_back(-size, size, size);	// 6
		vertices.emplace_back(size, size, size);	// 7
		vertices.emplace_back(-size, size, size);	// 8
		vertices.emplace_back(-size, size, -size);	// 9
		vertices.emplace_back(size, size, size);	// 10
		vertices.emplace_back(size, size, -size);	// 11
		vertices.emplace_back(-size, size, size);	// 12
		vertices.emplace_back(-size, size, -size);	// 13

		const auto ConvertCords = [](const float x, const float y) {
			return Vec2(x / 3.0f, y / 4.0f);
			};

		tCords.reserve(14);
		tCords.emplace_back(ConvertCords(0.0f, 1.0f)); // 0
		tCords.emplace_back(ConvertCords(0.0f, 2.0f)); // 1
		tCords.emplace_back(ConvertCords(1.0f, 1.0f)); // 2
		tCords.emplace_back(ConvertCords(1.0f, 2.0f)); // 3
		tCords.emplace_back(ConvertCords(2.0f, 1.0f)); // 4
		tCords.emplace_back(ConvertCords(2.0f, 2.0f)); // 5
		tCords.emplace_back(ConvertCords(3.0f, 1.0f)); // 6
		tCords.emplace_back(ConvertCords(3.0f, 2.0f)); // 7
		tCords.emplace_back(ConvertCords(2.0f, 0.0f)); // 8
		tCords.emplace_back(ConvertCords(1.0f, 0.0f)); // 9
		tCords.emplace_back(ConvertCords(2.0f, 3.0f)); // 10
		tCords.emplace_back(ConvertCords(1.0f, 3.0f)); // 11
		tCords.emplace_back(ConvertCords(2.0f, 4.0f)); // 12
		tCords.emplace_back(ConvertCords(1.0f, 4.0f)); // 13
	}

	IndexedLineList GetLines() const
	{
		return IndexedLineList{
			vertices,
			{0,1,  1,3,
			 2,3,  2,0,
			 6,7,  7,5,
			 5,4,  4,6,
			 0,6,  2,4,
			 1,7,  3,5}
		};
	}

	IndexedTriangleList<Vec3> GetTriangles() const
	{
		return IndexedTriangleList<Vec3>{
			vertices,
			{ 2,0,3, 1,3,0,
			 2,3,4, 5,4,3,
			 4,5,6, 7,6,5,
			 4,8,2, 9,2,8,
			 3,11,5, 10,5,11,
			 11,13,10, 12,10,13 }
		};
	}

	IndexedTriangleList<TexVertex> GetTrianglesTex() const
	{
		std::vector<TexVertex> texVertices;
		texVertices.reserve(vertices.size());

		for (size_t i = 0; i < vertices.size(); i++)
		{
			texVertices.emplace_back(vertices[i], tCords[i]);
		}

		return IndexedTriangleList<TexVertex>{
			std::move(texVertices),
			{ 2,0,3, 1,3,0,
			 2,3,4, 5,4,3,
			 4,5,6, 7,6,5,
			 4,8,2, 9,2,8,
			 3,11,5, 10,5,11,
			 11,13,10, 12,10,13 }
		};
	}

private:
	std::vector<Vec3> vertices;
	std::vector<Vec2> tCords;
};