#pragma once
#ifndef PISTOL_H
#define PISTOL_H
#include "Gun.h"

class Pistol : public Gun {
		public:
			Pistol() : Gun(0.5, 10, 1, 10) {}
};
#endif