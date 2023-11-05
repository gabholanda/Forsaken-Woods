#pragma once
#include "Gun.h"

class SniperRifle : public Gun {
public:
	SniperRifle() : Gun(0.2, 50, 3, 2) {}
};

