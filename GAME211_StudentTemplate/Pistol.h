#pragma once
#ifndef PISTOL_H
#define PISTOL_H
#include "Gun.h"

class Pistol : public Gun {
		public:
			Pistol() : Gun("Pistol", 0.3, 15, 1, 6) {}
};
#endif