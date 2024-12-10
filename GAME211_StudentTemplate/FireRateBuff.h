#pragma once
#include "Buff.h"

class FireRateBuff : public Buff
{

protected:

	float fireRateIncrement = 0.02;

public:
	void ApplyBuff(PlayerBody* player) override;
	std::string getName() const  { return "FireRate"; }

	FireRateBuff() : Buff("Fire rate increased") {}

};

