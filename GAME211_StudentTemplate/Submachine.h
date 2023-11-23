#pragma once
#ifndef SUBMACHINE_H
#define SUBMACHINE_H
#include "Gun.h"

class Submachine : public Gun {
public:
	Submachine() : Gun("SubMachine Gun",0.1f, 5, 2, 40) {}
};	
#endif

