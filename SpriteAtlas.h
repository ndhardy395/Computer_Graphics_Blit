#pragma once

struct SpriteRect
{
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int w = 0;
	unsigned int h = 0;
};

namespace TownAtlas
{
	//Grass
	const SpriteRect GRASS_TILE = { 288,128,16,16 };

	//Buildings
	const SpriteRect SHOP =			{ 16, 16, 96, 83 };
	const SpriteRect RED_HOUSE =	{ 128, 32, 64, 67 };
	const SpriteRect GREEN_HOUSE =  { 208, 32, 96, 65 };
	const SpriteRect STONE_HALL =	{ 16, 112, 144, 72 };

	//Scenery
	const SpriteRect TREE =			{ 320, 16, 64, 80 };
	const SpriteRect FENCE =		{ 212, 112, 40, 48 };
	const SpriteRect GRASS_PATCH =  { 272, 112, 64, 64 };
	const SpriteRect RED_BUSH =		{ 176, 112, 16, 24 };

	//Wooden signs
	const SpriteRect SIGN_A = { 176, 176, 16, 16 };
	const SpriteRect SIGN_B = { 192, 176, 16, 16 };
	const SpriteRect SIGN_C = { 208, 176, 16, 16 };

	//Bushes
	const SpriteRect BUSH_A =		{ 16, 192, 16, 16 };
	const SpriteRect BUSH_B =		{ 32, 192, 16, 16 };
	const SpriteRect FLOWER_BLUE =  { 48, 192, 16, 16 };
	const SpriteRect FLOWER_PINK =  { 64, 192, 16, 16 };
}