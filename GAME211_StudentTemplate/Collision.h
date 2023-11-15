#pragma once
#include "Body.h"
#include "CollisionTile.h"
#include "GameManager.h"


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

