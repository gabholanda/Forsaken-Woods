#pragma once
#include "Buff.h"
class DashCooldownBuff : public Buff
{
protected:

	float dashCooldownIncrement = 0.2;

public:
	void ApplyBuff(PlayerBody* player) override;
	std::string getName() const  { return "DashCooldown"; }

	DashCooldownBuff() : Buff("Dash cooldown decreased") {}
};

