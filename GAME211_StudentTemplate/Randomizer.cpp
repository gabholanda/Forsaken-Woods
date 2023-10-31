#include "Randomizer.h"
#include <random>

std::vector<Gun*> Randomizer::weapons = {};

Gun* Randomizer::getRandomWeapon()
{
	weapons = {
	new Pistol(),
	new Submachine()
	};
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distribution(0, weapons.size() - 1); 
	int index = distribution(gen);
	return weapons[index];

}

int Randomizer::RandomNumberForEnemy()
{
	std::vector<int> enemies{ 1, 2, 3 };
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(0, enemies.size() - 1);
	int index = distribution(gen);

	return enemies[index];
}

 Scene* Randomizer::RandomRoom(SDL_Window* sdlWindow, GameManager* game_)
{
	 std::vector<Scene*> rooms{
 new Scene1(sdlWindow, game_),
 new Scene2(sdlWindow, game_),
 new Scene3(sdlWindow, game_)
	 };
	 std::random_device rd;
	 std::mt19937 gen(rd());
	 std::uniform_int_distribution<> distribution(0, rooms.size() - 1);
	 int index = distribution(gen);
	 return rooms[index];
}
