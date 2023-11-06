#pragma once
#include "Buff.h"

class ReloadSpeedBuff : public Buff
{
protected:

	float reloadSpeedIncrement = 5;	

public:
	void ApplyBuff(PlayerBody* player) override;
};

