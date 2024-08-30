#pragma once
#include "Colors.h"
#include "Surface.h"
#include "Rect.h"

class Font
{
public:
	// Constructors
	Font(const std::wstring& fontFile, int nColumns, int nRows);

	// Methods
	RectI CharToRect(char c) const;
	Surface& GetSprite();
	int GetCWidth() const;
	int GetCHeight() const;
	Color GetColor() const;
	Color GetChroma() const;
	void SetColor(const Color& newColor);
	void SetChroma(const Color& newChroma);

private:
	Color color = Colors::White;
	Color chroma = Colors::White;
	Surface sprite;
	int nColumns;
	int nRows;
	int charWidth;
	int charHeight;

	static constexpr char firstChar = ' ';
	static constexpr char lastChar = '~';
};
