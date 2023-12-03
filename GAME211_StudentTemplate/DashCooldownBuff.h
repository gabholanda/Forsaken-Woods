#pragma once
#include "Buff.h"
class DashCooldownBuff : public Buff
{
protected:

	float dashCooldownIncrement = 0.2;

public:
	void ApplyBuff(PlayerBody* player) override;
	DashCooldownBuff() : Buff("Dash cooldown decreased") {}
};

