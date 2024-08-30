#include "Font.h"
#include "Rect.h"

Font::Font(const std::wstring& fontFile, int nColumns, int nRows)
	:
	sprite(Surface::FromFile(fontFile)),
	nColumns(nColumns),
	nRows(nRows)
{
	charWidth = sprite.GetWidth() / nColumns;
	charHeight = sprite.GetHeight() / nRows;
}

RectI Font::CharToRect(char c) const
{
	assert(c >= firstChar);
	assert(c <= lastChar);

	int offset = c - firstChar;
	int row = offset / nColumns;
	int column = offset - row * nColumns;

	return { column * charWidth, row * charHeight, charWidth, charHeight };
}

Surface& Font::GetSprite()
{
	return sprite;
}

int Font::GetCWidth() const
{
	return charWidth;
}

int Font::GetCHeight() const
{
	return charHeight;
}

Color Font::GetColor() const
{
	return color;
}

Color Font::GetChroma() const
{
	return chroma;
}

void Font::SetColor(const Color& newColor)
{
	color = newColor;
}

void Font::SetChroma(const Color& newChroma)
{
	chroma = newChroma;
}
