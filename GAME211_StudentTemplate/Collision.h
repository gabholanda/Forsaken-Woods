#pragma once
#include "Body.h"
#include "CollisionTile.h"
#include "GameManager.h"

//	https://developer.mozilla.org/en-US/docs/Games/Techniques/2D_collision_detection
//	https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-detection
//	https://toptal.com/game/video-game-physics-part-ii-collision-detection-for-solid-objects
// ChatGPT
static class Collision
{
public:
	static SDL_Surface* debugImage;
	static SDL_Texture* debugTexture;
	static bool CheckCollision(Body& A, Body& B);
	static void ResolveCollision(Body* A, Body* B);

	static void DisplayDebugCollision(Body& A, GameManager& manager);
	static void DisplayDebugCollision(CollisionTile& A, GameManager& manager);
};

