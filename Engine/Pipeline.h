#pragma once
#include "Vec3.h"
#include "Mat.h"
#include "IndexedTriangleList.h"
#include "Triangle.h"
#include "NDCScreenTransformer.h"
#include "ChiliMath.h"
#include "ZBuffer.h"
#include <algorithm>

template <class Effect>
class Pipeline
{
public:
	//Type of vertices which pipeline will take as input
	typedef typename Effect::Vertex Vertex;
	//type of vertices after VertexShader gets applied
	typedef typename Effect::VertexShader::Output VSOut;
	//type of vertices after GeometryShader gets applied
	typedef typename Effect::GeometryShader::Output GSOut;
public:
	Pipeline(Graphics& gfx) 
		:
		gfx(gfx),
		pZb(std::make_shared<ZBuffer>(gfx.ScreenWidth, gfx.ScreenHeight))
	{}

	Pipeline(Graphics& gfx, std::shared_ptr<ZBuffer> pZb)
		:
		gfx(gfx),
		pZb(std::move(pZb))
	{
		assert(this->pZb->GetHeight() == gfx.ScreenHeight && this->pZb->GetWidth() == gfx.ScreenWidth);
	}

	//Clears ZBuffer so new frame can be drawn
	void BeginFrame()
	{
		pZb->Clear();
	}

	void BindProjection(const Mat4& projection)
	{
		effect.vs.BindProjection(projection);

	}

	// public interface function to start pipeline process for indexedtrianglelist
	void Draw(const IndexedTriangleList<Vertex>& triList)
	{
		ProcessVertices(triList.vertices, triList.indices);
	}

private:
	// Apply Vertex Shader to each vertex
	void ProcessVertices(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices)
	{
		std::vector<VSOut> outputVertexStream(vertices.size());
		//outputVertexStream.reserve(vertices.size()+5);

		//Apply transformation to all vertices, result is VSOut, as vertexshader might change type
		std::transform(vertices.cbegin(), vertices.cend(), outputVertexStream.begin(), effect.vs);

		AssembleTriangles(outputVertexStream, indices);
	}

	// Assemble vertex stream and index stream into individual triangle objects which get processed
	// Backface Culling gets applied here
	void AssembleTriangles(const std::vector<VSOut>& vertices, const std::vector<size_t>& indices)
	{
		Vec4 eyePos = Vec4(0.0f, 0.0f, 0.0f, 1.0f) * effect.vs.GetProj();

		for (size_t i = 0; i < indices.size() / 3; i++)
		{
			const VSOut& v0 = vertices[indices[i * 3]];
			const VSOut& v1 = vertices[indices[i * 3 + 1]];
			const VSOut& v2 = vertices[indices[i * 3 + 2]];

			// Check for backface culling
			if ((v1.pos - v0.pos) % (v2.pos - v0.pos) * (v0.pos - eyePos) <= 0.0f)
			{
				ProcessTriangle(v0, v1, v2, i);
			}
		}
	}

	// Starting point to process a triangle
	// applies geometry shader
	// Takes 3 vertices to generate triangle on screen
	void ProcessTriangle(const VSOut& v0, const VSOut& v1, const VSOut& v2, size_t index)
	{
		// generate triangle from 3 vertices using gs
		// and send to post-processing
		PostProcessTriangleVertices(effect.gs( v0, v1, v2, index ));
	}

	// Apply Screen transformation to triangle and draw triangle
	void PostProcessTriangleVertices(Triangle<GSOut>& triangle)
	{
		screenTransformer.Transform(triangle.v0);
		screenTransformer.Transform(triangle.v1);
		screenTransformer.Transform(triangle.v2);

		DrawTriangle(triangle);
	}

	// === triangle rasterization functions ===
	//
	// entry point for tri rasterization
	// sorts vertices, determines case, splits to flat tris, dispatches to flat tri funcs
	void DrawTriangle(const Triangle<GSOut>& triangle)
	{
		//use pointers wo we can sort them
		const GSOut* pv0 = &triangle.v0;
		const GSOut* pv1 = &triangle.v1;
		const GSOut* pv2 = &triangle.v2;

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
		GSOut vi = interpolate(*pv0, *pv2, alpha);

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

	void DrawFlatTopTriangle(const GSOut& v0, const GSOut& v1, const GSOut& v2)
	{
		//Calculate slopes
		GSOut tDelta0 = (v2 - v0) / (v2.pos.y - v0.pos.y);
		GSOut tDelta1 = (v2 - v1) / (v2.pos.y - v1.pos.y);

		// calculate start and end scanlines
		int startY = ceil(v0.pos.y - 0.5f);
		int endY = ceil(v2.pos.y - 0.5f); // the scanline AFTER the last line drawn

		//Start function with given information
		DrawFlatTriangle(startY, endY, tDelta0, tDelta1, v0, v1);
	}

	void DrawFlatBottomTriangle(const GSOut& v0, const GSOut& v1, const GSOut& v2)
	{
		//Calculate slopes
		GSOut tDelta0 = (v1 - v0) / (v1.pos.y - v0.pos.y);
		GSOut tDelta1 = (v2 - v0) / (v2.pos.y - v0.pos.y);

		// calculate start and end scanlines
		int startY = ceil(v0.pos.y - 0.5f);
		int endY = ceil(v1.pos.y - 0.5f); // the scanline AFTER the last line drawn

		DrawFlatTriangle(startY, endY, tDelta0, tDelta1, v0, v0);
	}

	// Draw triangle Flat triangle, that is Flat Bottom or Flat Top
	// int startY and endY are screen coordinates where the triangle should be drawn, startY included and endY excluded
	// Vertex tDelta says how much Vertex changes per Y step, 0 for left side and 1 for right side
	// Vertex v0 and v1 define the two top points for the triangle, for Flatbottom we have v0 == v1 
	void DrawFlatTriangle(int startY, int endY, const GSOut vDelta0, const GSOut vDelta1,
		const GSOut v0, const GSOut v1)
	{
		//stores current position on the lines of the triangle (moves with increasing y)
		GSOut vCurrentY0 = v0 + vDelta0 * ((startY + 0.5f) - (v0.pos.y));
		GSOut vCurrentY1 = v1 + vDelta1 * ((startY + 0.5f) - (v0.pos.y));

		for (int y = startY; y < endY; y++, vCurrentY0 += vDelta0, vCurrentY1 += vDelta1)
		{
			//Skip if y is not on screen
			if (y < 0 || y >= gfx.ScreenHeight) continue;
			//Find start and end points
			int startX = ceil(vCurrentY0.pos.x - 0.5f);
			int endX = ceil(vCurrentY1.pos.x - 0.5f);

			//Get delta per x step and calculate start position for texture map only
			GSOut vDeltaX = (vCurrentY1 - vCurrentY0) / (vCurrentY1.pos.x - vCurrentY0.pos.x);
			GSOut vCurrentX = vCurrentY0 + vDeltaX * ((startX + 0.5f) - (vCurrentY0.pos.x));

			for (int x = startX; x < endX; x++, vCurrentX += vDeltaX)
			{
				//Skip if x is not on screen
				if (x < 0 || x >= gfx.ScreenWidth) continue;
				
				// Only draw Pixel if it passes zBuffer test, i.e. no pixel in front
				if (pZb->TestAndSet(x, y, vCurrentX.pos.z))
				{
					//Recover original zCoordinate again, as pos.w contains 1/z
					const float wCoordinate = 1.0f / vCurrentX.pos.w;

					//Get back correct uv coordinates by multiplying them with z
					GSOut recoveredCords = vCurrentX * wCoordinate;
					// wCoordinate is z coordinate if needed

					gfx.PutPixel(x, y, effect.ps(recoveredCords));
				}
			}
		}
	}
public:
	Effect effect;

private:
	Graphics& gfx;
	std::shared_ptr<ZBuffer> pZb;
	NDCScreenTransformer screenTransformer;
};