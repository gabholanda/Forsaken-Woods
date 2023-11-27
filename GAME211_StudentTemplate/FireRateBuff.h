#pragma once
#include "Buff.h"

class FireRateBuff : public Buff
{

protected:

	float fireRateIncrement = 0.1;

public:
	void ApplyBuff(PlayerBody* player) override;
};

