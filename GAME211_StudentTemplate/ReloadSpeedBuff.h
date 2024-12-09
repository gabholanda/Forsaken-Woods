#pragma once
#include "Buff.h"

class ReloadSpeedBuff : public Buff
{
protected:

	float reloadSpeedIncrement = 0.15;	

public:
	void ApplyBuff(PlayerBody* player) override;
	std::string getName() const  { return "ReloadSpeed"; }

	ReloadSpeedBuff() : Buff("Reload Speed Increased") {}

};

