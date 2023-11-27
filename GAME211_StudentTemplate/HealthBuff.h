#pragma once
#include "Buff.h"
class HealthBuff : public Buff
{
protected:

	float healthIncrement = 10;

public:
	void ApplyBuff(PlayerBody* player) override;
};

