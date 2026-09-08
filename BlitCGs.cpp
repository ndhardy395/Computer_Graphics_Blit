#include <iostream>
#include "RasterSurface.h"
#include "Surface.h"
#include <cstdlib>
#include <ctime>

//XTime timer;
//srand((unsigned int)time(nullptr));

int main()
{
	const char* studentName = "Noah Hardy";
	Surface screen(500, 500);


	RS_Initialize(studentName, screen.Width(), screen.Height());

	do
	{
		screen.Clear(0xFF202020);					// Dark grey
		screen.SetPixel(250, 250, 0xFFFF0000);		// One red pixel in the center
	} while (RS_Update(screen.Data(), screen.Count()));

	RS_Shutdown();
}
