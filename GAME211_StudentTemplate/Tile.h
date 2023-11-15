#pragma once
#include "GameObject.h"
#include "IRender.h"
#include "SDL.h"


class GameManager;
class Tile : public GameObject, public IRender
{
public:
	Tile();
	Tile(Vec3 pos_, float orientation_, float scale_, int rows_, int columns_, SDL_Rect sourceRect_, GameManager* manager_);
	void Render();

protected:
	int rows;
	int columns;
	GameManager* manager;
	SDL_Rect sourceRect;
};

