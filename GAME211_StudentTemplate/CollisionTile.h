#pragma once
#include "Tile.h"
#include "Body.h"

class CollisionTile : public Tile, public Body
{
public:
	CollisionTile();
	CollisionTile(Vec3 pos_, float orientation_, float scale_, int rows_, int columns_, Vec3 size, SDL_Rect sourceRect_, GameManager* manager_);
	void Render();
};

