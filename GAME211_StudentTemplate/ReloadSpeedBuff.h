#pragma once
#include "Buff.h"

class ReloadSpeedBuff : public Buff
{
protected:

	float reloadSpeedIncrement = 0.15;	

public:
	void ApplyBuff(PlayerBody* player) override;
	ReloadSpeedBuff() : Buff("Reload Speed Increased") {}

};

