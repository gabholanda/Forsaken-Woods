#include "Randomizer.h"
#include <random>
int Randomizer::RandomNumberForWeapon()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1, maxWeaponNumber);
    return distribution(gen);
}

int Randomizer::RandomNumberForEnemy() 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1, maxEnemyNumber);
    return distribution(gen);
}