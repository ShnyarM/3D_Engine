#pragma once
#include <cassert>
#include <limits>

class ZBuffer
{
public:
	ZBuffer(const int width, const int height)
		:
		width(width),
		height(height)
	{
		pBuffer = new float[width * height];
	}

	ZBuffer(const ZBuffer& buf) = delete;

	~ZBuffer()
	{
		delete[] pBuffer;
		pBuffer = nullptr;
	}

	ZBuffer& operator=(const ZBuffer& rhs) = delete;

	int GetHeight() const
	{
		return height;
	}

	int GetWidth() const
	{
		return width;
	}

	float At(const int x, const int y) const
	{
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);
		return pBuffer[y * width + x];
	}

	void Set(const int x, const int y, const float z)
	{
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);
		pBuffer[y * width + x] = z;
	}

	bool TestAndSet(const int x, const int y, const float z)
	{
		const float currentZ = At(x, y);
		if (z < currentZ)
		{
			Set(x, y, z);
			return true;
		}
		return false;
	}

	void Clear()
	{
		for (size_t i = 0; i < width * height; i++)
		{
			pBuffer[i] = std::numeric_limits<float>::infinity();
		}
	}

private:
	int width;
	int height;
	float* pBuffer = nullptr;
};