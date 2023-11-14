#pragma once
#include "SDL.h"
#include <vector>
#include <MMath.h>
#include <VMath.h>

class Tile;
class GameManager;

using namespace MATH;

class Grid
{
private:
	std::vector<Vec3> positions;
	std::vector<Tile*> tiles;
	GameManager* manager;
	//Window* window;
	int width;
	int height;
	int rows;
	int columns;
public:

	Grid(int width_, int height_, int rows_, int columns_, GameManager* manager_);
	std::vector<Tile*>* GetTiles() { return &tiles; }
	void PushTile(Tile* tile, int position);
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetRows() const { return rows; }
	int GetColumns() const { return columns; }
	void RenderGrid();
	void RenderDebugGrid();
};

