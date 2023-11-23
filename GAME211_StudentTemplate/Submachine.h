#pragma once
#ifndef SUBMACHINE_H
#define SUBMACHINE_H
#include "Gun.h"

class Submachine : public Gun {
public:
	Submachine() : Gun(0.5f, 5, 2, 40) {}
};
#endif

