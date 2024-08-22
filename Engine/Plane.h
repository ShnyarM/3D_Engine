#pragma once
#pragma once

#pragma once
#include <vector>
#include "Vec3.h"
#include "IndexedLineList.h"
#include "IndexedTriangleList.h"
#include "TexVertex.h"

class Plane
{
public:
	template<class V>
	static IndexedTriangleList<V> GetPlain(float size = 1.0f)
	{
		float side = 0.5f * size;

		std::vector<V> verts(4);

		verts[0].pos = Vec3(-side, -0.5f, -side);	// 0
		verts[1].pos = Vec3(-side, -0.5f, side);	// 1
		verts[2].pos = Vec3(side, -0.5f, side);		// 2
		verts[3].pos = Vec3(side, -0.5f, -side);	// 3

		verts[0].t = Vec2(0.0f, 1.0f);	// 0
		verts[1].t = Vec2(0.0f, 0.0f);	// 1
		verts[2].t = Vec2(1.0f, 0.0f);	// 2
		verts[3].t = Vec2(1.0f, 1.0f);	// 3

		return{
			std::move(verts),
			{ 0,1,3, 2,3,1 }	//Top
		};
	}

	template<class V>
	static IndexedTriangleList<V> GetSliced(float size = 1.0f, int slices = 1)
	{
		float side = 0.5f * size;
		float triangleSize = size / slices;
		float textureSize = 1.0f / slices;

		std::vector<V> verts( (slices+1) * (slices+1) );
		std::vector<size_t> indexing(slices*slices*2*3);

		// Create vertices
		for (int x = 0; x <= slices; x++)
		{
			for (int z = 0; z <= slices; z++)
			{
				verts[x * (slices + 1) + z].pos = Vec3(-side + x * triangleSize, -0.5f, side - z * triangleSize);
				verts[x * (slices + 1) + z].t = Vec2(x*textureSize, z*textureSize);
			}
		}

		// Create indexing
		int index = 0;
		for (int x = 0; x < slices; x++)
		{
			for (int z = 0; z < slices; z++)
			{
				indexing[index] = x * (slices + 1) + z;
				indexing[index + 1] = (x + 1) * (slices + 1) + z;
				indexing[index + 2] = x * (slices + 1) + z + 1;

				indexing[index + 3] = (x + 1) * (slices + 1) + z;
				indexing[index + 4] = (x + 1) * (slices + 1) + z + 1;
				indexing[index + 5] = x * (slices + 1) + z + 1;

				index += 6;
			}
		}

		return{
			std::move(verts),
			std::move(indexing)
		};
	}
};