#pragma once
#include "SDL.h"
#include <vector>
#include <MMath.h>
#include <VMath.h>

class Tile;
class CollisionTile;
class DecorationTile;
class GameManager;

using namespace MATH;

class Grid
{
private:
	std::vector<Vec3> positions;
	std::vector<Tile> tiles;
	std::vector<CollisionTile> collisionTiles;
	std::vector<DecorationTile> decorationTiles;
	GameManager* manager;
	//Window* window;
	int width;
	int height;
	int rows;
	int columns;
public:

	Grid(int width_, int height_, int rows_, int columns_, GameManager* manager_);
	std::vector<Tile>* GetTiles() { return &tiles; }
	std::vector<CollisionTile>* GetCollisionTiles() { return &collisionTiles; }
	std::vector<DecorationTile>* GetDecorationTiles() { return &decorationTiles; }
	void PushTile(Tile* tile, int position);
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetRows() const { return rows; }
	int GetColumns() const { return columns; }
	void RenderGrid();
	void RenderDebugGrid();

	void Clear();
};

