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
