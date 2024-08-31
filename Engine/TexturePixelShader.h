#pragma once
#include "Vec3.h"
#include "Colors.h"
#include <vector>

template <class Vertex>
class TexturePixelShader
{
public:
	// Get Color based of vertex and current loaded texture
	Color operator()(const Vertex& in)
	{
		assert(textureLoaded);
		return pTexture->GetPixel(
			(unsigned int)std::min(in.t.x * tWidth, tWidthMax),
			(unsigned int)std::min(in.t.y * tHeight, tHeightMax));
	}

	void BindTexture(const std::wstring& filename)
	{
		pTexture = std::make_unique<Surface>(Surface::FromFile(filename));
		tWidth = pTexture->GetWidth();
		tHeight = pTexture->GetHeight();
		tWidthMax = tWidth - 1.0f;
		tHeightMax = tHeight - 1.0f;
		textureLoaded = true;
	}

private:
	std::unique_ptr<Surface> pTexture;
	bool textureLoaded = false;
	//Helper variables for clamping
	float tWidth = 0.0f;
	float tHeight = 0.0f;
	float tWidthMax = 0.0f;
	float tHeightMax = 0.0f;
};
