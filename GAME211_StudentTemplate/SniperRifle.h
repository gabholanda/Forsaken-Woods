#pragma once
#include "Gun.h"

class SniperRifle : public Gun {
public:
	SniperRifle() : Gun("Sniper Rifle", 2.0f, 100, 4, 2) {}
};

