#pragma once
#include "SpriteAtlas.h"

class Surface;

namespace Scene
{
	constexpr int FIRE_X = 186;
	constexpr int FIRE_Y = 186;
	constexpr unsigned int FIRE_SIZE = 128;
}

struct Placement
// signed placement; Blit clips it.
{
	SpriteRect sprite;
	int x;
	int y;
};

bool RectsOverlap(int _ax, int _ay, int _aw, int _ah, int _bx, int _by, int _bw, int _bh);

// counts the hand-placed town elements
const Placement* TownLayout(unsigned int& _outCount);

unsigned int ScatterRandom(Placement* _results, unsigned int _count, unsigned int _screenW, unsigned int _screenH);

// Drawing with a Surface& onto _target
void DrawPlacements(Surface& _target, const Surface& _sheet, const Placement* _placements, unsigned int _count);