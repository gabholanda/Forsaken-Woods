#pragma once
#ifndef GUN_H
#define GUN_H

#include "SDL.h"
#include "EnemyBody.h"

class PlayerBody;
class EnemyBody;

class Gun {
protected:
	float fireRate;
	float damage;
	float reloadSpeed;
	bool canShoot;
	int currentAmmo;
	int maxAmmo;
	PlayerBody* gunOwner;
	PlayerBody* target;
	EnemyBody* gunOwnerEnemy;
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

	void SetEnemyGunOwner(EnemyBody* gunOwnerEnemy_)
	{
		gunOwnerEnemy = gunOwnerEnemy_;
	}

	virtual void ShootForEnemy(PlayerBody* target);

	//Uint32 StartFireRateTimer(Uint32 interval, void* param);

	virtual void Shoot();
	static Uint32 FireRateTimerCallback(Uint32 interval, void* param);
	static Uint32 ReloadTimerCallback(Uint32 interval, void* param);
	// Getters
	float GetFireRate() const {
		return fireRate;
	}

	int GetMaxAmmo() const {
		return maxAmmo;
	}

	float GetReloadSpeed() const {
		return reloadSpeed;
	}

	float GetDamage() const {
		return damage;
	}

	// Setters
	void SetFireRate(float fireRate_) {
		fireRate = fireRate_;
	}

	void SetMaxAmmo(int maxAmmo_) {
		maxAmmo = maxAmmo_;
	}

	void SetReloadSpeed(float reloadSpeed_) {
		reloadSpeed = reloadSpeed_;
	}

	void SetDamage(float damage_) {
		damage = damage_;
	}

};

#endif