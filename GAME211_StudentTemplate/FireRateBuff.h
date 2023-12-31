#pragma once
#include "Buff.h"

class FireRateBuff : public Buff
{

protected:

	float fireRateIncrement = 0.02;

public:
	void ApplyBuff(PlayerBody* player) override;
	FireRateBuff() : Buff("Fire rate increased") {}

};

