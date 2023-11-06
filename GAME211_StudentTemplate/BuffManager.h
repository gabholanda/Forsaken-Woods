#pragma once
#include <vector>
#include "DamageBuff.h"
#include "FireRateBuff.h"
#include "MovementSpeedBuff.h"
#include "ReloadSpeedBuff.h"
#include "HealthBuff.h"

class BuffManager
{	
private:
	std::vector<Buff*> buffs;
public:
	BuffManager() {
		buffs.push_back(new DamageBuff());
		buffs.push_back(new MovementSpeedBuff());
		buffs.push_back(new FireRateBuff());
		buffs.push_back(new ReloadSpeedBuff());
		buffs.push_back(new HealthBuff());
	}
	std::vector<Buff*> PickRandomBuff();
	 std::vector<Buff*> GetBuffs() const {
		return buffs;
	}
};

