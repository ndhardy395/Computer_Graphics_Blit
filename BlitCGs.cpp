#include <iostream>
#include "RasterSurface.h"
#include "Surface.h"
#include "XTime.h"
#include "SpriteAtlas.h"
#include "Scene.h"
#include "TownTiles.h"
#include "fire_02.h"

#include <cstdlib>
#include <ctime>

constexpr unsigned int FIRE_CELL = Scene::FIRE_SIZE;
constexpr unsigned int FIRE_COLUMNS = fire_02_width / FIRE_CELL;
constexpr unsigned int FIRE_ROWS = fire_02_height / FIRE_CELL;
constexpr unsigned int FIRE_FRAMES = FIRE_COLUMNS * FIRE_ROWS;

int main()
{
	const char* studentName = "Noah Hardy - Lab 1";

	const unsigned int SCREEN_WIDTH = 500;
	const unsigned int SCREEN_HEIGHT = 500;
	const unsigned int RANDOM_COUNT = 10;

	const double SECONDS_PER_FRAME = 1.0 / 30.0;

	srand((unsigned int)time(nullptr));

	Surface screen(SCREEN_WIDTH, SCREEN_HEIGHT);
	Surface townTiles(towntiles_pixels, towntiles_width, towntiles_height);
	Surface fire(fire_02_pixels, fire_02_width, fire_02_height);

	Placement scattered[RANDOM_COUNT];
	unsigned int scatteredCount = ScatterRandom(scattered, RANDOM_COUNT, SCREEN_WIDTH, SCREEN_HEIGHT);
	unsigned int townCount = 0;
	const Placement* town = TownLayout(townCount);

	XTime timer;
	double frameAccumulator = 0.0;
	unsigned int animationFrame = 0;


	RS_Initialize(studentName, screen.Width(), screen.Height());
	//do
	//{
	//	screen.Clear(0xFF202020);					// Dark grey
	//	screen.SetPixel(250, 250, 0xFFFF0000);		// One red pixel in the center
	//} while (RS_Update(screen.Data(), screen.Count()));
	do
	{
		timer.Signal();
		frameAccumulator += timer.Delta();

		while (frameAccumulator >= SECONDS_PER_FRAME)
		{
			frameAccumulator -= SECONDS_PER_FRAME;
			animationFrame = (animationFrame + 1) % FIRE_FRAMES;
		}

		screen.Tile(townTiles, TownAtlas::GRASS_TILE.x, TownAtlas::GRASS_TILE.y,
			TownAtlas::GRASS_TILE.w, TownAtlas::GRASS_TILE.h);

		DrawPlacements(screen, townTiles, town, townCount);
		DrawPlacements(screen, townTiles, scattered, scatteredCount);

		unsigned int cellX = (animationFrame % FIRE_COLUMNS) * FIRE_CELL;
		unsigned int cellY = (animationFrame / FIRE_COLUMNS) * FIRE_CELL;

		screen.Blit(fire, Scene::FIRE_X, Scene::FIRE_Y, cellX, cellY, FIRE_CELL, FIRE_CELL);

	} while (RS_Update(screen.Data(), screen.Count()));

	RS_Shutdown();

	return 0;
}
