#include "Collision.h"

SDL_Surface* Collision::debugImage = nullptr;
SDL_Texture* Collision::debugTexture = nullptr;

bool Collision::CheckCollision(Body& A, Body& B)
{
	// Calculate the coordinates of the edges of the squares
	float leftA = A.getPos().x - A.getSize().x / 2.0f;
	float rightA = A.getPos().x + A.getSize().x / 2.0f;
	float topA = A.getPos().y - A.getSize().y / 2.0f;
	float bottomA = A.getPos().y + A.getSize().x / 2.0f;

	float leftB = B.getPos().x - B.getSize().x / 2.0f;
	float rightB = B.getPos().x + B.getSize().x / 2.0f;
	float topB = B.getPos().y - B.getSize().y / 2.0f;
	float bottomB = B.getPos().y + B.getSize().x / 2.0f;

	// Check for collision along the x-axis
	bool collisionX = rightA >= leftB && leftA <= rightB;

	// Check for collision along the y-axis
	bool collisionY = bottomA >= topB && topA <= bottomB;

	return collisionX && collisionY;
}

void Collision::ResolveCollision(Body* A, Body* B)
{
	float leftA = A->getPos().x - A->getSize().x / 2.0f;
	float rightA = A->getPos().x + A->getSize().x / 2.0f;
	float topA = A->getPos().y - A->getSize().y / 2.0f;
	float bottomA = A->getPos().y + A->getSize().x / 2.0f;

	float leftB = B->getPos().x - B->getSize().x / 2.0f;
	float rightB = B->getPos().x + B->getSize().x / 2.0f;
	float topB = B->getPos().y - B->getSize().y / 2.0f;
	float bottomB = B->getPos().y + B->getSize().x / 2.0f;
	// Calculate overlap along the x-axis and y-axis
	float overlapX = std::min(rightA, rightB) - std::max(leftA, leftB);
	float overlapY = std::min(bottomA, bottomB) - std::max(topA, topB);

	Vec3 resolution = Vec3();
	// Determine the direction of resolution based on overlap
	if (overlapX < overlapY) {
		resolution.x = (rightA < rightB) ? -overlapX : overlapX;
	}
	else {
		resolution.y = (bottomA < bottomB) ? -overlapY : overlapY;
	}

	A->setPos(A->getPos() + resolution);
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

	square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	square.y = static_cast<int>(screenCoords.y - 0.5f * h);
	square.w = static_cast<int>(w);
	square.h = static_cast<int>(h);

	// Convert character orientation from radians to degrees.
	float orientationDegrees = 0;

	SDL_RenderCopyEx(manager.getRenderer(), debugTexture, nullptr, &square,
		orientationDegrees, nullptr, SDL_FLIP_NONE);
}

void Collision::DisplayDebugCollision(CollisionTile& A, GameManager& manager)
{
	// square represents the position and dimensions for where to draw the image
	SDL_Rect square;
	Vec3 screenCoords;
	float    w, h;

	// convert the position from game coords to screen coords.
	screenCoords = manager.getProjectionMatrix() * A.Body::getPos();

	// Scale the image, in case the .png file is too big or small
	w = debugImage->w * A.Body::getSize().x * 0.166f;
	h = debugImage->h * A.Body::getSize().y * 0.166f;

	square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	square.y = static_cast<int>(screenCoords.y - 0.5f * h);
	square.w = static_cast<int>(w);
	square.h = static_cast<int>(h);

	// Convert character orientation from radians to degrees.
	float orientationDegrees = 0;

	SDL_RenderCopyEx(manager.getRenderer(), debugTexture, nullptr, &square,
		orientationDegrees, nullptr, SDL_FLIP_NONE);
}
