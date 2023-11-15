#include "GameManager.h"
#include "CollisionTile.h"

CollisionTile::CollisionTile()
{
	Tile::pos = Vec3();
	Body::pos = Vec3();
	GameObject::pos = Vec3();
	Tile::orientation = 0;
	Body::orientation = 0;
	GameObject::orientation = 0;
	Tile::scale = 0;
	Body::scale = 0;
	GameObject::scale = 0;
	size = Vec3(1, 1, 0);
}

CollisionTile::CollisionTile(Vec3 pos_, float orientation_, float scale_, int rows_, int columns_, Vec3 size_, SDL_Rect sourceRect_, GameManager* manager_)
{
	Tile::pos = pos_;
	Body::pos = pos_;
	GameObject::pos = pos_;
	Tile::orientation = orientation_;
	Body::orientation = orientation_;
	GameObject::orientation = orientation_;
	Tile::scale = scale_;
	Body::scale = scale_;
	GameObject::scale = scale_;
	size = size_;
	rows = rows_;
	columns = columns_;
	sourceRect = sourceRect_;
	manager = manager_;
}

void CollisionTile::Render()
{
	SDL_Renderer* renderer = manager->getRenderer();
	Matrix4 projectionMatrix = manager->getProjectionMatrix();

	// square represents the position and dimensions for where to draw the image
	SDL_Rect square;
	Vec3 screenCoords;
	float    w, h;

	// convert the position from game coords to screen coords.
	screenCoords = projectionMatrix * Tile::pos;

	// Scale the image, in case the .png file is too big or small
	w = static_cast<float>(Tile::image->w) / rows * Tile::scale;
	h = static_cast<float>(Tile::image->h) / columns * Tile::scale;
	sourceRect.w = w;
	sourceRect.h = h;
	// The square's x and y values represent the top left corner of 
	// where SDL will draw the .png image.
	// The 0.5f * w/h offset is to place the .png so that pos represents the center
	// (Note the y axis for screen coords points downward, hence subtraction!!!!)
	square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	square.y = static_cast<int>(screenCoords.y - 0.5f * h);
	square.w = static_cast<int>(w);
	square.h = static_cast<int>(h);

	// Convert character orientation from radians to degrees.
	float orientationDegrees = Tile::orientation * 180.0f / M_PI;

	SDL_RenderCopyEx(renderer, Tile::texture, &sourceRect, &square,
		orientationDegrees, nullptr, SDL_FLIP_NONE);
}
