#include "BuffManager.h"
#include <random>

std::vector<Buff*> BuffManager::PickRandomBuff()
{
    std::vector<Buff*> randomBuffs;
    if (buffs.empty()) {
        return randomBuffs; // Return an empty vector if no buffs are available
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, buffs.size() - 1);

    int randomIndex = distrib(gen);
    randomBuffs.push_back(buffs[randomIndex]);

    return randomBuffs;
}
