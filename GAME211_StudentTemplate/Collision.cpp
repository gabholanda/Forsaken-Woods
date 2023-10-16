#include "Collision.h"

SDL_Surface* Collision::debugImage = nullptr;
SDL_Texture* Collision::debugTexture = nullptr;

bool Collision::CheckCollision(Body& A, Body& B)
{
	// collision x-axis?
	bool collisionX = A.getPos().x + A.getSize().x >= B.getPos().x &&
		B.getPos().x + B.getSize().x >= A.getPos().x;
	// collision y-axis?
	bool collisionY = A.getPos().y + A.getSize().y >= B.getPos().y &&
		B.getPos().y + B.getSize().y >= A.getPos().y;
	// collision only if on both axes
	return collisionX && collisionY;
}

void Collision::DisplayDebugCollision(Body& A, GameManager& manager)
{
	// square represents the position and dimensions for where to draw the image
	SDL_Rect square;
	Vec3 screenCoords;
	float    w, h;

	// convert the position from game coords to screen coords.
	screenCoords = manager.getProjectionMatrix() * A.getPos();

	// Scale the image, in case the .png file is too big or small
	w = debugImage->w * A.getSize().x * 0.166f;
	h = debugImage->h * A.getSize().y * 0.166f;

	// The square's x and y values represent the top left corner of 
	// where SDL will draw the .png image.
	// The 0.5f * w/h offset is to place the .png so that pos represents the center
	// (Note the y axis for screen coords points downward, hence subtraction!!!!)
	square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	square.y = static_cast<int>(screenCoords.y - 0.5f * h);
	square.w = static_cast<int>(w);
	square.h = static_cast<int>(h);

	// Convert character orientation from radians to degrees.
	float orientationDegrees = 0;

	SDL_RenderCopyEx(manager.getRenderer(), debugTexture, nullptr, &square,
		orientationDegrees, nullptr, SDL_FLIP_NONE);
}
