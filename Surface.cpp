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
	mWidth = _width;
	mHeight = _height;

	if (mWidth == 0 || mHeight == 0)
	{
		mWidth = 0;
		mHeight = 0;
		mPixels = nullptr;
		return;
	}
	mPixels = new unsigned int[mWidth * mHeight]();
}

Surface::Surface(const unsigned int* _source, unsigned int _width, unsigned int _height)
{
	mWidth = _width;
	mHeight = _height;

	if (_source == nullptr || mWidth == 0 || mHeight == 0)
	{
		mWidth = 0;
		mHeight = 0;
		mPixels = nullptr;
		return;
	}

	mPixels = new unsigned int[mWidth * mHeight];

	for (unsigned int point = 0; point < mWidth * mHeight; ++point)
	{
		mPixels[point] = SourceToXRGB(_source[point]);
	}
}

Surface::~Surface()
{
	delete[] mPixels;
	mPixels = nullptr;
}

Surface::Surface(const Surface& _copy)
{
	mWidth = _copy.mWidth;
	mHeight = _copy.mHeight;
	mPixels = nullptr;

	if (_copy.mPixels != nullptr)
	{
		mPixels = new unsigned int[mWidth * mHeight];

		for (unsigned int point = 0; point < mWidth * mHeight; ++point)
		{
			mPixels[point] = _copy.mPixels[point];
		}
	}
}

Surface& Surface::operator=(const Surface& _assign)
{
	if (this != &_assign)
	{
		delete[] mPixels;

		mWidth = _assign.mWidth;
		mHeight = _assign.mHeight;
		mPixels = nullptr;

		if (_assign.mPixels != nullptr)
		{
			mPixels = new unsigned int[mWidth * mHeight];

			for (unsigned int point = 0; point < mWidth * mHeight; ++point)
			{
				mPixels[point] = _assign.mPixels[point];
			}
		}
	}
	return *this;
}

unsigned int Surface::Index(unsigned int _x, unsigned int _y) const
{
	return _y * mWidth + _x;
}

unsigned int& Surface::operator()(unsigned int _x, unsigned int _y)
{
	return mPixels[Index(_x, _y)];
}

const unsigned int& Surface::operator()(unsigned int _x, unsigned int _y) const
{
	return mPixels[Index(_x, _y)];
}

unsigned int Surface::Width() const
{
	return mWidth;
}

unsigned int Surface::Height() const
{
	return mHeight;
}

unsigned int Surface::Count() const
{
	return mWidth * mHeight;
}

const unsigned int* Surface::Data() const
{
	return mPixels;
}

void Surface::Clear(unsigned int _color)
{
	for (unsigned int point = 0; point < mWidth * mHeight; ++point)
	{
		mPixels[point] = _color;
	}
}

void Surface::SetPixel(int _x, int _y, unsigned int _color)
{
	if (_x < 0 || _y < 0)
	{
		return;
	}
	if ((unsigned int)_x >= mWidth || (unsigned int)_y >= mHeight)
	{
		return;
	}
	mPixels[Index((unsigned int)_x, (unsigned int)_y)] = _color;
}

void Surface::BlendPixel(int _x, int _y, unsigned int _color)
{
	unsigned int alpha = (_color & 0xFF000000) >> 24;

	if (alpha == 0)
		return;

	if (alpha == 255)
	{
		SetPixel(_x, _y, _color);
		return;
	}

	if (_x < 0 || _y < 0)
		return;

	if ((unsigned int)_x >= mWidth || (unsigned int)_y >= mHeight)
		return;

	unsigned int& destination = mPixels[Index((unsigned int)_x, (unsigned int)_y)];

	//! Unpack each channel before blending
	unsigned int sourceRed = (_color & 0x00FF0000) >> 16;
	unsigned int sourceGreen = (_color & 0x0000FF00) >> 8;
	unsigned int sourceBlue = (_color & 0x000000FF);

	unsigned int destRed = (destination & 0x00FF0000) >> 16;
	unsigned int destGreen = (destination & 0x0000FF00) >> 8;
	unsigned int destBlue = (destination & 0x000000FF);

	unsigned int inverse = 255 - alpha;

	unsigned int red = (sourceRed * alpha + destRed * inverse) / 255;
	unsigned int green = (sourceGreen * alpha + destGreen * inverse) / 255;
	unsigned int blue = (sourceBlue * alpha + destBlue * inverse) / 255;

	destination = 0xFF000000 | (red << 16) | (green << 8) | blue;
}

void Surface::Blit(const Surface& _src, 
	int _destX, int _destY,
	unsigned int _srcX, unsigned int _srcY,
	unsigned int _cellW, unsigned int _cellH)
{
	for (unsigned int row = 0; row < _cellH; ++row)
	{
		unsigned int sourceY = _srcY + row;

		if (sourceY >= _src.mHeight)
			break;

		for (unsigned int column = 0; column < _cellW; ++column)
		{
			unsigned int sourceX = _srcX + column;

			if (sourceX >= _src.mWidth)
				break;

			unsigned int color = _src.mPixels[_src.Index(sourceX, sourceY)];

			BlendPixel(_destX + (int)column, _destY + (int)row, color);
		}
	}
}

void Surface::Tile(const Surface& _src, 
	unsigned int _srcX, unsigned int _srcY, 
	unsigned int _cellW, unsigned int _cellH)
{
	if (_cellW == 0 || _cellH == 0)
		return;

	for (unsigned int y = 0; y < mHeight; y += _cellH)
	{
		for (unsigned int x = 0; x < mWidth; x += _cellW)
		{
			Blit(_src, (int)x, (int)y, _srcX, _srcY, _cellW, _cellH);
		}
	}
}
