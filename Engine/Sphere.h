#pragma once
#include "IndexedTriangleList.h"
#include "Mat3.h"

class Sphere
{
public:
	template<class V>
	static IndexedTriangleList<V> GetPlain(float radius, int nLatitude, int nLongitude)
	{
		assert(nLatitude >= 2);
		assert(nLongitude >= 2);
		Vec3 base = { 0.0f, radius, 0.0f };
		float angleLatitude = PI / nLatitude;
		float angleLongitude = (2.0f * PI) / nLongitude;

		std::vector<V> vertices;
		std::vector<size_t> indices;

		// create vertices
		for (int iLatitude = 1; iLatitude <= nLatitude; iLatitude++)
		{
			Vec3 latitudeBase = base * Mat3::RotationX(angleLatitude * iLatitude);

			for (int iLongitude = 0; iLongitude < nLongitude; iLongitude++)
			{
				vertices.emplace_back();
				vertices.back().pos = latitudeBase * Mat3::RotationY(angleLongitude * iLongitude);
			}
		}

		// add top and bottom
		const int topIndex = vertices.size();
		vertices.emplace_back();
		vertices.back().pos = base;
		const int bottomIndex = topIndex + 1;
		vertices.emplace_back();
		vertices.back().pos = -base;

		// create indices
		// make "mantel"
		auto getIndex = [nLatitude, nLongitude](int iLatitude, int iLongitude) {
			return (iLatitude - 1) * nLongitude + iLongitude;
		};

		for (int iLatitude = 1; iLatitude < nLatitude-1; iLatitude++)
		{
			for (int iLongitude = 0; iLongitude < nLongitude-1; iLongitude++)
			{
				indices.push_back(getIndex(iLatitude, iLongitude));
				indices.push_back(getIndex(iLatitude + 1, iLongitude));
				indices.push_back(getIndex(iLatitude, iLongitude + 1));

				indices.push_back(getIndex(iLatitude, iLongitude + 1));
				indices.push_back(getIndex(iLatitude + 1, iLongitude));
				indices.push_back(getIndex(iLatitude + 1, iLongitude + 1));
			}
		}

		//make top and bottom
		for (int i = 0; i < nLongitude-1; i++)
		{
			// top
			indices.push_back(topIndex);
			indices.push_back(i);
			indices.push_back(i+1);

			// bottom
			indices.push_back(bottomIndex);
			indices.push_back((nLatitude-2) * nLongitude + i + 1);
			indices.push_back((nLatitude-2) * nLongitude + i);
		}

		// zip together
		// top
		indices.push_back(topIndex);
		indices.push_back(nLongitude-1);
		indices.push_back(0);

		//bottom
		indices.push_back(bottomIndex);
		indices.push_back((nLatitude - 2) * nLongitude);
		indices.push_back((nLatitude - 2) * nLongitude + nLongitude - 1);

		//rest
		for (int i = 1; i < nLatitude-1; i++)
		{
			indices.push_back(getIndex(i, nLongitude - 1));
			indices.push_back(getIndex(i+1, nLongitude - 1));
			indices.push_back(getIndex(i, 0));

			indices.push_back(getIndex(i, 0));
			indices.push_back(getIndex(i + 1, nLongitude - 1));
			indices.push_back(getIndex(i+1, 0));
		}

		return { std::move(vertices), std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> GetPlainNormal(float radius, int nLatitude, int nLongitude)
	{
		IndexedTriangleList<V> sphere = GetPlain<V>(radius, nLatitude, nLongitude);

		for (int i = 0; i < sphere.vertices.size(); i++)
		{
			sphere.vertices[i].n = sphere.vertices[i].pos.GetNormalized();
		}

		return sphere;
	}
};