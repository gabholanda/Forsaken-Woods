#pragma once
#ifndef GUN_H
#define GUN_H

#include "SDL.h"

class PlayerBody;

class Gun {
protected:
	float fireRate;
	float damage;
	float reloadSpeed;
	bool canShoot;
	int currentAmmo;
	int maxAmmo;
	PlayerBody* gunOwner;
	SDL_TimerID timerId;

public:
	Gun(float fireRate_, float damage_, float reloadSpeed_, int ammo_)
		: fireRate(fireRate_), damage(damage_), reloadSpeed(reloadSpeed_), maxAmmo(ammo_)
	{
		canShoot = true;
		gunOwner = nullptr;
		currentAmmo = maxAmmo;
		timerId = 0;
	}

	~Gun();

	void SetGunOwner(PlayerBody* gunOwner_)
	{
		gunOwner = gunOwner_;
	}

	//Uint32 StartFireRateTimer(Uint32 interval, void* param);

	virtual void Shoot();
	static Uint32 FireRateTimerCallback(Uint32 interval, void* param);
	static Uint32 ReloadTimerCallback(Uint32 interval, void* param);
};

#endif