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
	template<class V>
	static IndexedTriangleList<V> GetSkinned(float size = 1.0f)
	{
		float side = 0.5f * size;
		const auto ConvertCords = [](const float x, const float y) {
			return Vec2(x / 3.0f, y / 4.0f);
		}; 

		std::vector<V> verts(14);

		verts[0].pos = Vec3(-side, side, -side);	// 0
		verts[1].pos = Vec3(side, side, -side);		// 1
		verts[2].pos = Vec3(-side, -side, -side);	// 2
		verts[3].pos = Vec3(side, -side, -side);	// 3
		verts[4].pos = Vec3(-side, -side, side);	// 4
		verts[5].pos = Vec3(side, -side, side);		// 5
		verts[6].pos = Vec3(-side, side, side);		// 6
		verts[7].pos = Vec3(side, side, side);		// 7
		verts[8].pos = Vec3(-side, side, side);		// 8
		verts[9].pos = Vec3(-side, side, -side);	// 9
		verts[10].pos = Vec3(side, side, side);		// 10
		verts[11].pos = Vec3(side, side, -side);	// 11
		verts[12].pos = Vec3(-side, side, side);	// 12
		verts[13].pos = Vec3(-side, side, -side);	// 13

		verts[0].t = ConvertCords(0.0f, 1.0f);	// 0
		verts[1].t = ConvertCords(0.0f, 2.0f);	// 1
		verts[2].t = ConvertCords(1.0f, 1.0f);	// 2
		verts[3].t = ConvertCords(1.0f, 2.0f);	// 3
		verts[4].t = ConvertCords(2.0f, 1.0f);	// 4
		verts[5].t = ConvertCords(2.0f, 2.0f);	// 5
		verts[6].t = ConvertCords(3.0f, 1.0f);	// 6
		verts[7].t = ConvertCords(3.0f, 2.0f);	// 7
		verts[8].t = ConvertCords(2.0f, 0.0f);	// 8
		verts[9].t = ConvertCords(1.0f, 0.0f);	// 9
		verts[10].t = ConvertCords(2.0f, 3.0f); // 10
		verts[11].t = ConvertCords(1.0f, 3.0f); // 11
		verts[12].t = ConvertCords(2.0f, 4.0f); // 12
		verts[13].t = ConvertCords(1.0f, 4.0f); // 13

		return{
			std::move(verts),
			{ 2,0,3, 1,3,0,			//Front
			 2,3,4, 5,4,3,			//Bot
			 4,5,6, 7,6,5,			//Back
			 4,8,2, 9,2,8,			//Left
			 3,11,5, 10,5,11,		//Right
			 11,13,10, 12,10,13 }	//Top
		};
	}

	template <class V>
	static IndexedTriangleList<V> GetPlain(float size = 1.0f)
	{
		float side = 0.5f * size;

		std::vector<V> verts(8);

		verts[0].pos = Vec3(-side, side, -side);	// 0
		verts[1].pos = Vec3(side, side, -side);		// 1
		verts[2].pos = Vec3(-side, -side, -side);	// 2
		verts[3].pos = Vec3(side, -side, -side);	// 3
		verts[4].pos = Vec3(-side, -side, side);	// 4
		verts[5].pos = Vec3(side, -side, side);		// 5
		verts[6].pos = Vec3(-side, side, side);		// 6
		verts[7].pos = Vec3(side, side, side);		// 7

		return{
			std::move(verts),
			{ 2,0,3, 1,3,0,		//Front
			 2,3,4, 5,4,3,		//Bot
			 4,5,6, 7,6,5,		//Back
			 4,6,2, 0,2,6,		//Left
			 3,1,5, 7,5,1,		//Right
			 1,0,7, 6,7,0 }		//Top
		};
	}

	template <class V>
	static IndexedTriangleList<V> GetPlainIndependentFaces(float size = 1.0f)
	{
		float side = 0.5f * size;

		std::vector<V> verts(24);

		verts[0].pos = Vec3(-side, side, -side);	// 0
		verts[1].pos = Vec3(side, side, -side);		// 1
		verts[2].pos = Vec3(-side, -side, -side);	// 2
		verts[3].pos = Vec3(side, -side, -side);	// 3
		verts[4].pos = Vec3(-side, -side, side);	// 4
		verts[5].pos = Vec3(side, -side, side);		// 5
		verts[6].pos = verts[2].pos;				// 6
		verts[7].pos = verts[3].pos;				// 7
		verts[8].pos = Vec3(-side, side, side);		// 8
		verts[9].pos = Vec3(side, side, side);		// 9
		verts[10].pos = verts[4].pos;				// 10
		verts[11].pos = verts[5].pos;				// 11
		verts[12].pos = verts[8].pos;				// 12
		verts[13].pos = verts[4].pos;				// 13
		verts[14].pos = verts[0].pos;				// 14
		verts[15].pos = verts[2].pos;				// 15
		verts[16].pos = verts[5].pos;				// 16
		verts[17].pos = verts[9].pos;				// 17
		verts[18].pos = verts[3].pos;				// 18
		verts[19].pos = verts[1].pos;				// 19
		verts[20].pos = verts[9].pos;				// 20
		verts[21].pos = verts[8].pos;				// 21
		verts[22].pos = verts[1].pos;				// 22
		verts[23].pos = verts[0].pos;				// 23

		return{
			std::move(verts),
			{ 2,0,3,    1,3,0,		//Front
			  4,6,5,    7,5,6,		//Bot
			  8,10,9,	11,9,10,	//Back
			  15,13,14, 12,14,13,	//Left
			  18,19,16, 17,16,19,	//Right
			  22,23,20, 21,20,23 }	//Top
		};
	}
};