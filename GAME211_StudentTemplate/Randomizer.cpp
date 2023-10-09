#include "Randomizer.h"
#include <random>
int Randomizer::RandomNumberForWeapon()
{
	std::vector<int> weapons{ 1, 2, 3 };
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distribution(0, weapons.size() - 1); // Range from 0 to size-1
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