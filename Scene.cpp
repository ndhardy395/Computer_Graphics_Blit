#include "Scene.h"
#include "Surface.h"

using namespace TownAtlas;

static const Placement gTown[] =
{
    { SHOP, 20, 30 },
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
    return nullptr;
}

unsigned int ScatterRandom(Placement* _results, unsigned int _count, unsigned int _screenW, unsigned int _screenH)
{
    return 0;
}

void DrawPlacement(Surface& _target, const Surface& _sheet, const Placement* _placements, unsigned int _count)
{

}
