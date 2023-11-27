#include "Grid.h"
#include <SDL_image.h>
#include "Tile.h"
#include "CollisionTile.h"
#include "DecorationTile.h"
#include "GameManager.h"

Grid::Grid(int width_, int height_, int rows_, int columns_, GameManager* manager_)
{
	manager = manager_;
	width = width_;
	height = height_;
	rows = rows_;
	columns = columns_;

	for (int i = 0; i < columns; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			positions.push_back({ (float)(j * width / 40.0f), (float)(i * height / 40.0f), 0.0f });
		}
	}
}

void Grid::PushTile(Tile* tile, int position)
{
	if (position > positions.size())
	{
		return;
	}

	if (tile == nullptr)
	{
		return;
	}

	if (CollisionTile* collisionTile = dynamic_cast<CollisionTile*>(tile))
	{
		collisionTile->Body::setPos(positions[position]);
		collisionTile->Tile::setPos(positions[position]);
		collisionTile->GameObject::setPos(positions[position]);
		collisionTiles.push_back(*collisionTile);
		return;
	}

	if (DecorationTile* decorationTile = dynamic_cast<DecorationTile*>(tile))
	{
		decorationTile->Tile::setPos(positions[position]);
		decorationTile->GameObject::setPos(positions[position]);
		decorationTiles.push_back(*decorationTile);
		return;
	}

	Tile newTile = *tile;
	newTile.setPos(positions[position]);
	tiles.push_back(newTile);
}

void Grid::RenderGrid()
{
	for (Tile tile : tiles)
	{
		tile.Render();
	}

	for (Tile tile : collisionTiles)
	{
		tile.Render();
	}

	for (Tile tile : decorationTiles)
	{
		tile.Render();
	}
}

void Grid::RenderDebugGrid()
{
	for (size_t i = 0; i < positions.size(); i++)
	{
		SDL_Renderer* renderer = manager->getRenderer();
		Matrix4 projectionMatrix = manager->getProjectionMatrix();
		Vec3 screenCoords = projectionMatrix * positions[i];
		SDL_Rect screenRect;
		screenRect.x = static_cast<int>(screenCoords.x - 0.5f * width);
		screenRect.y = static_cast<int>(screenCoords.y - 0.5f * height);
		// Render with an offset for better visiblity
		screenRect.w = static_cast<int>(width - 5);
		screenRect.h = static_cast<int>(height - 5);
		// Draw the square.
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set color to white
		SDL_RenderFillRect(renderer, &screenRect);
	}
}

void Grid::Clear()
{
	tiles.clear();
	collisionTiles.clear();
	decorationTiles.clear();
}
