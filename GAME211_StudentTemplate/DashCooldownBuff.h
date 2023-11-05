#pragma once
#include "Buff.h"
class DashCooldownBuff : public Buff
{
protected:

	float dashCooldownIncrement = 5;

public:
	void ApplyBuff(PlayerBody* player) override;
};

