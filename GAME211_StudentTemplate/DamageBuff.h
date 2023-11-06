#pragma once
#include "Buff.h"

class DamageBuff : public Buff
{

protected:

	float damageIncrement = 5;

public:
	void ApplyBuff(PlayerBody* player);
};

