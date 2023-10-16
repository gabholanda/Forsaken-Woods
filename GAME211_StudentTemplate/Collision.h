#pragma once
#include "Body.h"
#include "GameManager.h"


static class Collision
{
public:
	static SDL_Surface* debugImage;
	static SDL_Texture* debugTexture;
	static bool CheckCollision(Body& A, Body& B);

	static void DisplayDebugCollision(Body& A, GameManager& manager);
};

