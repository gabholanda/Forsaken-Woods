#pragma once
#ifndef RANDOMIZER_H
#define RANDOMIZER_H
#include "Gun.h"
#include "Pistol.h"
#include "Submachine.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"

static class Randomizer
{	
public:
	static Gun* getRandomWeapon();
	int RandomNumberForEnemy();
	static Scene* RandomRoom(SDL_Window* sdlWindow, GameManager* game_);
private:
	static std::vector<Gun*> weapons;
};

#endif