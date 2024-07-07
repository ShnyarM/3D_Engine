#pragma once
#include "Vec2.h"
#include "Vec3.h"
#include "Mat3.h"
#include "IndexedTriangleList.h"
#include "Triangle.h"
#include "ScreenTransformer.h"
#include "ChiliMath.h"
#include <algorithm>

class Pipeline
{
public:
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(const Vec3& pos, const Vec2& tPos)
			:
			pos(pos),
			t(tPos)
		{
		}
		Vertex& operator=(const Vertex& rhs)
		{
			pos = rhs.pos;
			t = rhs.t;
			return *this;
		}
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			t += rhs.t;
			return *this;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			t -= rhs.t;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(const float rhs)
		{
			pos *= rhs;
			t *= rhs;
			return *this;
		}
		Vertex operator*(const float rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(const float rhs)
		{
			pos /= rhs;
			t /= rhs;
			return *this;
		}
		Vertex operator/(const float rhs) const
		{
			return Vertex(*this) /= rhs;
		}

		Vec3 pos;
		Vec2 t;
	};

public:
	Pipeline(Graphics& gfx) 
		:
		gfx(gfx)
	{}

	// public interface function to start pipeline process for indexedtrianglelist
	void Draw(const IndexedTriangleList<Vertex>& triList)
	{
		ProcessVertices(triList.vertices, triList.indices);
	}
	void BindRotation(const Mat3& rotation_in)
	{
		rotation = rotation_in;
	}
	void BindTranslation(const Vec3& translation_in)
	{
		translation = translation_in;
	}
	void BindTexture(const std::wstring& filename)
	{
		pTexture = std::make_unique<Surface>(Surface::FromFile(filename));
	}

private:
	// Apply World Space rotation and translation to each vertex
	void ProcessVertices(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		std::vector<Vertex> outputVertexStream;
		outputVertexStream.reserve(vertices.size());

		for (auto& vertex : vertices)
		{
			outputVertexStream.emplace_back((vertex.pos * rotation) + translation, vertex.t);
		}

		AssembleTriangles(outputVertexStream, indices);
	}

	// Assemble vertex stream and index stream into individual triangle objects which get processed
	// Backface Culling gets applied here
	void AssembleTriangles(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		for (size_t i = 0; i < indices.size() / 3; i++)
		{
			const Vertex& v0 = vertices[indices[i * 3]];
			const Vertex& v1 = vertices[indices[i * 3 + 1]];
			const Vertex& v2 = vertices[indices[i * 3 + 2]];

			// Check for backface culling
			if ((v1.pos - v0.pos) % (v2.pos - v0.pos) * v0.pos <= 0.0f)
			{
				ProcessTriangle(v0, v1, v2);
			}
		}
	}

	// Starting point to process a triangle
	// Takes 3 vertices to generate triangle on screen
	void ProcessTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		// generate triangle from 3 vertices using gs
		// and send to post-processing
		PostProcessTriangleVertices(Triangle<Vertex>{ v0, v1, v2 });
	}

	// Apply Screen transformation to triangle and draw triangle
	void PostProcessTriangleVertices(Triangle<Vertex>& triangle)
	{
		screenTransformer.Transform(triangle.v0.pos);
		screenTransformer.Transform(triangle.v1.pos);
		screenTransformer.Transform(triangle.v2.pos);

		DrawTriangle(triangle);
	}

	// === triangle rasterization functions ===
	//
	// entry point for tri rasterization
	// sorts vertices, determines case, splits to flat tris, dispatches to flat tri funcs
	void DrawTriangle(const Triangle<Vertex>& triangle)
	{
		//use pointers wo we can sort them
		const Vertex* pv0 = &triangle.v0;
		const Vertex* pv1 = &triangle.v1;
		const Vertex* pv2 = &triangle.v2;

		//Sort so pv0 is at the top
		if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);
		if (pv2->pos.y < pv1->pos.y) std::swap(pv1, pv2);
		if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);

		//Check if flat top
		if (pv0->pos.y == pv1->pos.y)
		{
			if (pv0->pos.x > pv1->pos.x) std::swap(pv0, pv1); //Sort so pv0 is left
			DrawFlatTopTriangle(*pv0, *pv1, *pv2);
			return;
		}

		//Check if flat bottom
		if (pv1->pos.y == pv2->pos.y)
		{
			if (pv2->pos.x < pv1->pos.x) std::swap(pv1, pv2); //Sort so pv1 is left
			DrawFlatBottomTriangle(*pv0, *pv1, *pv2);
			return;
		}

		//Split into flat top and flat bottom triangle
		float alpha = (pv1->pos.y - pv0->pos.y) / (pv2->pos.y - pv0->pos.y);
		Vertex vi = interpolate(*pv0, *pv2, alpha);

		if (vi.pos.x > pv1->pos.x) //major right
		{
			DrawFlatBottomTriangle(*pv0, *pv1, vi);
			DrawFlatTopTriangle(*pv1, vi, *pv2);
		}
		else //Major left
		{
			DrawFlatBottomTriangle(*pv0, vi, *pv1);
			DrawFlatTopTriangle(vi, *pv1, *pv2);
		}
	}

	void DrawFlatTopTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		//Calculate slopes
		Vertex tDelta0 = (v2 - v0) / (v2.pos.y - v0.pos.y);
		Vertex tDelta1 = (v2 - v1) / (v2.pos.y - v1.pos.y);

		// calculate start and end scanlines
		int startY = ceil(v0.pos.y - 0.5f);
		int endY = ceil(v2.pos.y - 0.5f); // the scanline AFTER the last line drawn

		//Start function with given information
		DrawFlatTriangle(startY, endY, tDelta0, tDelta1, v0, v1);
	}

	void DrawFlatBottomTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
	{
		//Calculate slopes
		Vertex tDelta0 = (v1 - v0) / (v1.pos.y - v0.pos.y);
		Vertex tDelta1 = (v2 - v0) / (v2.pos.y - v0.pos.y);

		// calculate start and end scanlines
		int startY = ceil(v0.pos.y - 0.5f);
		int endY = ceil(v1.pos.y - 0.5f); // the scanline AFTER the last line drawn

		DrawFlatTriangle(startY, endY, tDelta0, tDelta1, v0, v0);
	}

	// Draw triangle Flat triangle, that is Flat Bottom or Flat Top
	// int startY and endY are screen coordinates where the triangle should be drawn, startY included and endY excluded
	// Vertex tDelta says how much Vertex changes per Y step, 0 for left side and 1 for right side
	// Vertex v0 and v1 define the two top points for the triangle, for Flatbottom we have v0 == v1 
	void DrawFlatTriangle(int startY, int endY, const Vertex vDelta0, const Vertex vDelta1,
		const Vertex v0, const Vertex v1)
	{
		//stores current position on the lines of the triangle (moves with increasing y)
		Vertex vCurrentY0 = v0 + vDelta0 * ((startY + 0.5f) - (v0.pos.y));
		Vertex vCurrentY1 = v1 + vDelta1 * ((startY + 0.5f) - (v0.pos.y));

		//Helper variables for clamping
		const float tWidth = pTexture->GetWidth();
		const float tHeight = pTexture->GetHeight();
		const float tWidthMax = tWidth - 1.0f;
		const float tHeightMax = tHeight - 1.0f;

		for (int y = startY; y < endY; y++, vCurrentY0 += vDelta0, vCurrentY1 += vDelta1)
		{
			//Find start and end points
			int startX = ceil(vCurrentY0.pos.x - 0.5f);
			int endX = ceil(vCurrentY1.pos.x - 0.5f);

			//Get delta per x step and calculate start position for texture map only
			Vertex vDeltaX = (vCurrentY1 - vCurrentY0) / (vCurrentY1.pos.x - vCurrentY0.pos.x);
			Vertex vCurrentX = vCurrentY0 + vDeltaX * ((startX + 0.5f) - (vCurrentY0.pos.x));

			for (int x = startX; x < endX; x++, vCurrentX += vDeltaX)
			{
				gfx.PutPixel(x, y, pTexture->GetPixel(
					(unsigned int)std::min(vCurrentX.t.x * tWidth, tWidthMax),
					(unsigned int)std::min(vCurrentX.t.y * tHeight, tHeightMax)));
			}
		}
	}

private:
	Graphics& gfx;
	ScreenTransformer screenTransformer;
	Vec3 translation;
	Mat3 rotation = Mat3::Identity();
	std::unique_ptr<Surface> pTexture;
};