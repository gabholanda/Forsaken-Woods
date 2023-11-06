#pragma once
#include "Buff.h"
class HealthBuff : public Buff
{
protected:

	float healthIncrement = 5;

public:
	void ApplyBuff(PlayerBody* player) override;
};

