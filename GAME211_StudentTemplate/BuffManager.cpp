#include "BuffManager.h"
#include <random>

int BuffManager::PickRandomBuffIndex()
{
    if (buffs.empty()) {
        return -1; // Return -1 if no buffs are available
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, buffs.size() - 1);

    return distrib(gen);
}

Buff* BuffManager::randomBuff()
{
    int number = rand()% 5;
    switch (number)
    {
    case 0: return new DamageBuff();
        break;
    case 1: return new MovementSpeedBuff();
        break;
    case 2: return new FireRateBuff();
        break;
    case 3: return new ReloadSpeedBuff();
        break;
    case 4: return new HealthBuff();
        break;
    default:
        break;
    }
}
