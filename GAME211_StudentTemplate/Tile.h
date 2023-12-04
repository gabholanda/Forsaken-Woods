#pragma once
#include "GameObject.h"
#include "IRender.h"
#include "SDL.h"


class GameManager;
class Tile : public GameObject, public IRender
{
protected:
	int rows;
	int columns;
	GameManager* manager;
	SDL_Rect sourceRect;

public:
	Tile();
	~Tile()
	{
		image = nullptr;
		texture = nullptr;
	}
	Tile(Vec3 pos_, float orientation_, float scale_, int rows_, int columns_, SDL_Rect sourceRect_, GameManager* manager_);
	void Render();
	int getRows() { return rows; }
	int getColumns() { return columns; }
	SDL_Rect getSourceRect() { return sourceRect; }
	GameManager* getManager() { return manager; }
};

