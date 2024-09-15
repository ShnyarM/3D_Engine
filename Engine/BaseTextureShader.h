#pragma once
#include "Vec3.h"
#include "Colors.h"
#include <vector>
#include <memory>
#include "Surface.h"

class BaseTextureShader
{
public:
	// Get Color based on vertex and current loaded texture
	template<class Input>
	Color GetColor(const Input& in)
	{
		assert(textureLoaded);
		return pTexture->GetPixel(
			(unsigned int)(in.t.x * tWidth) % (unsigned int)tWidth,
			(unsigned int)(in.t.y * tHeight) % (unsigned int)tHeight);
	}

	void BindTexture(Surface* pNewSurface)
	{
		pTexture = pNewSurface;
		tWidth = pTexture->GetWidth();
		tHeight = pTexture->GetHeight();
		tWidthMax = tWidth - 1.0f;
		tHeightMax = tHeight - 1.0f;
		textureLoaded = true;
	}

private:
	Surface* pTexture;
	bool textureLoaded = false;
	//Helper variables for clamping
	float tWidth = 0.0f;
	float tHeight = 0.0f;
	float tWidthMax = 0.0f;
	float tHeightMax = 0.0f;
};
