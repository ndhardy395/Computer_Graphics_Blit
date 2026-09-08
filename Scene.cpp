#include "Scene.h"
#include "Surface.h"
#include <cstdlib>

using namespace TownAtlas;

static const Placement gTown[] =
{
	{SHOP, 20, 30 },
	{SIGN_A, 120, 97},
	{RED_HOUSE, 140, 46},
	{GREEN_HOUSE, 230, 48},
	{SIGN_C, 330, 97},
	{TREE, 350, 33},

	{STONE_HALL, 30, 396},
	{GREEN_HOUSE, 200, 403},
	{FENCE, 320, 420},
	{SIGN_B, 374, 452},
	{FLOWER_PINK, 398, 452},
	{TREE, 428, 388},
};

bool RectsOverlap(int _ax, int _ay, int _aw, int _ah, int _bx, int _by, int _bw, int _bh)
{
	return _ax < _bx + _bw && _bx < _ax + _aw && _ay < _by + _bh && _by < _ay + _ah;
}

const Placement* TownLayout(unsigned int& _outCount)
{
	_outCount = sizeof(gTown) / sizeof(gTown[0]);
	return gTown;
}

unsigned int ScatterRandom(Placement* _results, unsigned int _count, unsigned int _screenW, unsigned int _screenH)
{
	const SpriteRect pool[] = { TREE, TREE, TREE, GRASS_PATCH, RED_BUSH, BUSH_A, BUSH_B, FLOWER_BLUE };
	const unsigned int poolSize = sizeof(pool) / sizeof(pool[0]);

	const unsigned int MAX_ATTEMPTS = 400;

	unsigned int townCount = 0;
	const Placement* town = TownLayout(townCount);

	unsigned int placed = 0;

	for (unsigned int i = 0; i < _count; ++i)
	{
		for (unsigned int attempt = 0; attempt < MAX_ATTEMPTS; ++attempt) 
		{
			SpriteRect chosen = pool[rand() % poolSize];

			int halfW = (int)chosen.w / 2;
			int halfH = (int)chosen.h / 2;

			int x = (rand() % (int)_screenW) - halfW;
			int y = (rand() % (int)_screenW) - halfH;

			bool blocked = RectsOverlap(x, y, (int)chosen.w, (int)chosen.h, Scene::FIRE_X, Scene::FIRE_Y, 
				(int)Scene::FIRE_SIZE, (int)Scene::FIRE_SIZE);

			for (unsigned int t = 0; t < townCount && !blocked; ++t)
			{
				blocked = RectsOverlap(x, y, (int)chosen.w, (int)chosen.h, town[t].x, town[t].y,
					(int)town[t].sprite.w, (int)town[t].sprite.h);
			}
			for (unsigned int p = 0; p < placed && !blocked; ++p)
			{
				blocked = RectsOverlap(x, y, (int)chosen.w, (int)chosen.h, _results[p].x, _results[p].y,
					(int)_results[p].sprite.w, (int)_results[p].sprite.h);
			}

			if (!blocked)
			{
				_results[placed].sprite = chosen;
				_results[placed].x = x;
				_results[placed].y = y;
				++placed;
				break;
			}
		}
	}
	return placed;
}

void DrawPlacement(Surface& _target, const Surface& _sheet, const Placement* _placements, unsigned int _count)
{
	for (unsigned int i = 0; i < _count; ++i)
	{
		_target.Blit(_sheet, _placements[i].x, _placements[i].y,
			_placements[i].sprite.x, _placements[i].sprite.y,
			_placements[i].sprite.w, _placements[i].sprite.h);
	}
}
