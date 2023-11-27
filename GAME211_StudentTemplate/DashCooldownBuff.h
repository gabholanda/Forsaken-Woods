#pragma once
#include "Buff.h"
class DashCooldownBuff : public Buff
{
protected:

	float dashCooldownIncrement = 0.1;

public:
	void ApplyBuff(PlayerBody* player) override;
};

