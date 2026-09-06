#include <iostream>
#include "RasterSurface.h"

int main()
{
	//const char[] name = { 'N', 'o', 'a', 'h', ' ', 'H', 'a', 'r', 'd', 'y' };
	const char* studentName = "Noah Hardy";
	const unsigned int width = 500;
	const unsigned int height = 500;

	RS_Initialize(studentName, width, height);

	bool RS_Update(const unsigned int* _xrgbPixels, const unsigned int _numPixels);

	bool RS_Shutdown(void);
}
