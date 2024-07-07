#pragma once
#include "Vec3.h"
#include "Vec2.h"
#include "Colors.h"
#include <memory>
#include "Surface.h"

class TextureEffect
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
		{}
		Vertex(const Vec3& pos, const Vertex& v)
			:
			pos(pos),
			t(v.t)
		{}
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

	class PixelShader
	{
	public:
		// Input class is Vertex
		// Get Color based of vertex and current loaded texture
		template <class Input>
		Color operator()(const Input& in)
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

public:
	PixelShader ps;
};