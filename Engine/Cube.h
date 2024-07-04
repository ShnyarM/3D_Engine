#pragma once
#include <vector>
#include "Vec3.h"
#include "IndexedLineList.h"

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

private:
	std::vector<Vec3> vertices;
};