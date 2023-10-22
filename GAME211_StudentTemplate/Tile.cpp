#include "Tile.h"
#include "GameManager.h"

Tile::Tile()
{
	pos = Vec3();
	orientation = 0;
	scale = 0;
}

Tile::Tile(Vec3 pos_, float orientation_, float scale_, int rows_, int columns_, SDL_Rect sourceRect_, GameManager* manager_)
{
	pos = pos_;
	orientation = orientation_;
	scale = scale_;
	rows = rows_;
	columns = columns_;
	sourceRect = sourceRect_;
	manager = manager_;
}

void Tile::Render()
{
	SDL_Renderer* renderer = manager->getRenderer();
	Matrix4 projectionMatrix = manager->getProjectionMatrix();

	// square represents the position and dimensions for where to draw the image
	SDL_Rect square;
	Vec3 screenCoords;
	float    w, h;

	// convert the position from game coords to screen coords.
	screenCoords = projectionMatrix * pos;

	// Scale the image, in case the .png file is too big or small
	w = static_cast<float>(image->w) / rows * scale;
	h = static_cast<float>(image->h) / columns * scale;
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
	float orientationDegrees = orientation * 180.0f / M_PI;

	SDL_RenderCopyEx(renderer, texture, &sourceRect, &square,
		orientationDegrees, nullptr, SDL_FLIP_NONE);
}
