#pragma once
#include "Tile.h"
class DecorationTile : public Tile
{
public:
	DecorationTile(Vec3 pos_, float orientation_, float scale_, int rows_, int columns_, SDL_Rect sourceRect_, GameManager* manager_) :
		Tile(pos_, orientation_, scale_, rows_, columns_, sourceRect_, manager_)
	{

	}
};

