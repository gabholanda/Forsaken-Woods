#pragma once
#include "Gun.h"

class SniperRifle : public Gun {
public:
	SniperRifle() : Gun("Sniper Rifle", 1.0f, 100, 3, 2) {}
};

