#pragma once
#include "Gun.h"

class SniperRifle : public Gun {
public:
	SniperRifle() : Gun("Sniper Rifle", 2.0f, 100, 2.5, 2) {}
};

