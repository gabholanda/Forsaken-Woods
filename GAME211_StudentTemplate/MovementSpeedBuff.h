#pragma once
#include "Buff.h"
class MovementSpeedBuff : public Buff
{
protected:

	float movementSpeedIncrement = 0.5;

public:
	void ApplyBuff(PlayerBody* player) override;
	std::string getName() const  { return "MovementSpeed"; }

	MovementSpeedBuff() : Buff("Movespeed Increased") {}

};

