#pragma once
#include "Buff.h"

class DamageBuff : public Buff
{

protected:

	float damageIncrement = 2;

public:
	DamageBuff() : Buff("Damage increased"){}
	std::string getName() const { return "Damage"; }
	void ApplyBuff(PlayerBody* player);

};

