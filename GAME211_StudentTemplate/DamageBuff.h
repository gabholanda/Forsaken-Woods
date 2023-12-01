#pragma once
#include "Buff.h"

class DamageBuff : public Buff
{

protected:

	float damageIncrement = 3;

public:
	void ApplyBuff(PlayerBody* player);
};

