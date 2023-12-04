#pragma once
#include "Buff.h"
class MovementSpeedBuff : public Buff
{
protected:

	float movementSpeedIncrement = 0.5;

public:
	void ApplyBuff(PlayerBody* player) override;
	MovementSpeedBuff() : Buff("Movespeed Increased") {}

};

