#include "Surface.h"

// 0xBBGGRRAA -> 0xAARRGGBB
// A full four-byte reversal
unsigned int SourceToXRGB(unsigned int _source)
{
	unsigned int blue = (_source & 0xFF000000) >> 24;
	unsigned int green = (_source & 0x00FF0000) >> 16;
	unsigned int red = (_source & 0x0000FF00) >> 8;
	unsigned int alpha = (_source & 0x000000FF);

	return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

Surface::Surface(unsigned int _width, unsigned int _height)
{

}

Surface::Surface(const unsigned int* _source, unsigned int _width, unsigned int _height)
{

}

Surface::~Surface()
{

}

Surface::Surface(const Surface& _copy)
{

}

Surface& Surface::operator=(const Surface& _assign)
{
	// TODO: insert return statement here
}

unsigned int Surface::Index(unsigned int _x, unsigned int _y) const
{
	return 0;
}

unsigned int& Surface::operator()(unsigned int _x, unsigned int _y)
{
	// TODO: insert return statement here
}

const unsigned int& Surface::operator()(unsigned int _x, unsigned int _y) const
{
	// TODO: insert return statement here
}

unsigned int Surface::Width() const
{
	return 0;
}

unsigned int Surface::Height() const
{
	return 0;
}

unsigned int Surface::Count() const
{
	return mWidth * mHeight;
}

const unsigned int* Surface::Data() const
{
	return nullptr;
}

void Surface::Clear(unsigned int _color)
{

}

void Surface::SetPixel(int _x, int _y, unsigned int _color)
{

}

void Surface::BlendPixel(int _x, int _y, unsigned int _color)
{

}

void Surface::Blit(const Surface& _src, int _destX, int _destY, unsigned int _srcX, unsigned int _srcY, unsigned int _cellW, unsigned int _cellH)
{

}

void Surface::Tile(const Surface& _src, unsigned int _srcX, unsigned int _srcY, unsigned int _cellW, unsigned int _cellH)
{

}
