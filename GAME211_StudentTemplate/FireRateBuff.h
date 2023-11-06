#pragma once
#include "Buff.h"

class FireRateBuff : public Buff
{

protected:

	float fireRateIncrement;

public:
	void ApplyBuff(PlayerBody* player) override;
};

