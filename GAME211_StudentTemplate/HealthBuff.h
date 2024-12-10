#pragma once
#include "Buff.h"
class HealthBuff : public Buff
{
protected:

	float healthIncrement = 20;

public:
	void ApplyBuff(PlayerBody* player) override;
	std::string getName() const  { return "Health"; }

	HealthBuff() : Buff("Health Increased") {}

};

