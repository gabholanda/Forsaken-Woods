#include "Grid.h"
#include <SDL_image.h>
#include "Tile.h"
#include "CollisionTile.h"
#include "DecorationTile.h"
#include "GameManager.h"

Grid::Grid(int width_, int height_, int rows_, int columns_, GameManager* manager_, PlayerCamera* camera_)
{
	manager = manager_;
	width = width_;
	height = height_;
	rows = rows_;
	columns = columns_;
	camera = camera_;

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
		CollisionTile newCollisionTile = CollisionTile(collisionTile->Body::getPos(),
			collisionTile->Body::getOrientation(),
			collisionTile->Body::getScale(),
			collisionTile->getRows(),
			collisionTile->getColumns(),
			collisionTile->getSize(),
			collisionTile->getSourceRect(),
			collisionTile->getManager());
		newCollisionTile.Tile::setImage(collisionTile->Tile::getImage());
		newCollisionTile.Tile::setTexture(collisionTile->Tile::getTexture());
		collisionTiles.push_back(newCollisionTile);
		return;
	}

	if (DecorationTile* decorationTile = dynamic_cast<DecorationTile*>(tile))
	{
		decorationTile->Tile::setPos(positions[position]);
		decorationTile->GameObject::setPos(positions[position]);
		DecorationTile newDecorationTile = DecorationTile(decorationTile->getPos(),
			decorationTile->getOrientation(),
			decorationTile->getScale(),
			decorationTile->getRows(),
			decorationTile->getColumns(),
			decorationTile->getSourceRect(),
			decorationTile->getManager());
		newDecorationTile.setImage(decorationTile->getImage());
		newDecorationTile.setTexture(decorationTile->getTexture());
		decorationTiles.push_back(newDecorationTile);
		return;
	}

	Tile newTile = Tile(tile->getPos(),
		tile->getOrientation(),
		tile->getScale(),
		tile->getRows(),
		tile->getColumns(),
		tile->getSourceRect(),
		tile->getManager());
	newTile.setPos(positions[position]);
	newTile.setImage(tile->getImage());
	newTile.setTexture(tile->getTexture());
	tiles.push_back(newTile);
}

int Grid::GetTileIndex(Vec3 position)
{
	for (size_t i = 0; i < positions.size(); ++i)
	{
		if (positions[i] == position)
		{
			return static_cast<int>(i);
		}
	}
	return -1;
}

void Grid::RenderGrid() {
    for (Tile& tile : tiles) {
        if (isTileInView(tile.getPos())) {
            tile.Render();
        }
    }

    for (size_t i = decorationTiles.size() - 1; i > 0; i--) {
        if (isTileInView(decorationTiles[i].getPos())) {
            decorationTiles[i].Render();
        }
    }
}

void Grid::RenderCollisionTiles() {
	for (Tile tile : collisionTiles)
	{
		if (isTileInView(tile.getPos())) {
			tile.Render();
		}
	}
}

bool Grid::isTileInView(const Vec3& position) const {
	Vec3 cameraCenter = camera->getCameraCenter();
	Vec2 cameraBounds = camera->getCameraBounds();

	const float margin = 2.0f; // extend past bounds of camera for seamless rendering
	float minX = cameraCenter.x - cameraBounds.x / 2.0f - margin;
	float maxX = cameraCenter.x + cameraBounds.x / 2.0f + margin;
	float minY = cameraCenter.y - cameraBounds.y / 2.0f - margin;
	float maxY = cameraCenter.y + cameraBounds.y / 2.0f + margin;

	return (position.x >= minX && position.x <= maxX &&
		position.y >= minY && position.y <= maxY);
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

std::vector<Tile*> Grid::GetValidTiles(Vec3 playerPosition, float playerSpawnIndex)
{
	std::vector<Tile*> validTiles;

	const int exclusionRange = 5;
	const int additionalExclusion = 10;

	for (size_t i = 0; i < tiles.size(); ++i)
	{
		if (i < playerSpawnIndex - exclusionRange ||
			i > playerSpawnIndex + exclusionRange ||
			(i > playerSpawnIndex - exclusionRange - additionalExclusion &&
				i < playerSpawnIndex + exclusionRange + additionalExclusion))
		{
			validTiles.push_back(&tiles[i]);
		}
	}

	return validTiles;
}

void Grid::Clear()
{
	tiles.clear();
	collisionTiles.clear();
	decorationTiles.clear();
}
