#pragma once
#include "Gun.h"
class AssaultRifle : public Gun
{
public:
	AssaultRifle() : Gun("Assault Rifle", 0.75f, 30, 2.5, 20) {}
};

