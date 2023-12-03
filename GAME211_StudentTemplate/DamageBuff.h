#pragma once
#include "Buff.h"

class DamageBuff : public Buff
{

protected:

	float damageIncrement = 3;

public:
	DamageBuff() : Buff("Damage increased"){}
	void ApplyBuff(PlayerBody* player);

};

