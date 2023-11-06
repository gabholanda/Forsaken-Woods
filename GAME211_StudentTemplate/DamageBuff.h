#pragma once
#include "Buff.h"
class Gun;

class DamageBuff : public Buff
{

protected:

	float damageIncrement = 5;

public:
	void ApplyBuff(PlayerBody* player);
};

