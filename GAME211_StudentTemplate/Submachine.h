#pragma once
#ifndef SUBMACHINE_H
#define SUBMACHINE_H
#include "Gun.h"

class Submachine : public Gun {
public:
	Submachine() : Gun(3, 15, 1.5, 32) {}
};
#endif

