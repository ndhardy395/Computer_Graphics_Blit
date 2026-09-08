#pragma once

unsigned int SourceToXRGB(unsigned int _source);

class Surface
{
private:
	/// Data Members
	unsigned int* mPixels = nullptr;
	unsigned int mWidth = 0;
	unsigned int mHeight = 0;
public:

	// Creates an empty surface
	Surface(unsigned int _width = 0, unsigned int _height = 0);

	// Creates a surface from texture header ~ 0xBBGGRRAA -> 0xAARRGGBB
	Surface(const unsigned int* _source, unsigned int _width, unsigned int _height);

	~Surface();
	Surface(const Surface& _copy);
	Surface& operator=(const Surface& _assign);

	// 2D coords to 1D index
	unsigned int Index(unsigned int _x, unsigned int _y) const;

	//unchecked element access
	unsigned int& operator()(unsigned int _x, unsigned int _y);
	const unsigned int& operator()(unsigned int _x, unsigned int _y) const;

	// Accessors
	unsigned int Width() const;
	unsigned int Height() const;
	unsigned int Count() const;
	const unsigned int* Data() const;

	void Clear(unsigned int _color);

	void SetPixel(int _x, int _y, unsigned int _color);

	// Interpolation
	void BlendPixel(int _x, int _y, unsigned int _color);

	// Copies a rectangle out of _src onto this surface at _destX and _destY. 
	// Clipped on all four edges, so a sprite can hang off any side.
	void Blit(const Surface& _src, int _destX, int _destY,
		unsigned int _srcX, unsigned int _srcY,
		unsigned int _cellW, unsigned int _cellH);

	// Repeats an instance of _src with a const Surface& across the entire surface
	void Tile(const Surface& _src,
		unsigned int _srcX, unsigned int _srcY,
		unsigned int _cellW, unsigned int _cellH);
};