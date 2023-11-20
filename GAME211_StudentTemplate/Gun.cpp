#include "Bullet.h"
#include "EnemyBullet.h"
#include "Gun.h"
#include <MMath.h>
#include "PlayerBody.h"
#include "SDL.h"
#include <VMath.h>

Gun::~Gun()
{
	delete gunOwner;
	delete gunOwnerEnemy;
}

void Gun::ShootForEnemy(PlayerBody* target)
{
	if (gunOwnerEnemy == nullptr)
	{
		std::cerr << "GunOwner is nullptr, set it properly before calling Shoot function" << std::endl;
		return;
	}

	if (!canShoot)
	{
		return;
	}

	if (GameManager* manager = gunOwnerEnemy->GetGame())
	{
	
		Vec3 direction = VMath::normalize(target->getPos() - gunOwnerEnemy->getPos());
		float desiredAngle = std::atan2(direction.y, direction.x);
		float orientation = -desiredAngle;

		float mass = 1.0f;
		float rotation = 0.0f;
		float angular = 0.0f;
		float movementSpeed = 20.0f;
		float scale = 0.5f;
		float lifeTime = 2.f;

		Vec3 size(0.5f, 0.5f, 0.0f);
		Vec3 position = gunOwnerEnemy->getPos() + direction * 1.2f;
		Vec3 velocity(0.0f, 0.0f, 0.0f);
		Vec3 acceleration(0.0f, 0.0f, 0.0f);

		EnemyBullet* enemyBullet = new EnemyBullet(
			this,
			position,
			velocity,
			acceleration,
			size,
			mass,
			orientation,
			rotation,
			angular,
			movementSpeed,
			scale,
			lifeTime,
			manager);
		enemyBullet->OnCreate();
		enemyBullet->SetDirection(direction);
		manager->getEnemyBullets()->push_back(enemyBullet);
		canShoot = false;
		// Timer is in ms, while our value is in seconds
		timerId = SDL_AddTimer(fireRate * 1000.f, FireRateTimerCallback, reinterpret_cast<Gun*>(this));
		if (timerId = 0)
		{
			std::cerr << "Error on setting fireRate timer" << std::endl;
			canShoot = true;
			return;
		}
	}
}

void Gun::Shoot()
{

	if (gunOwner == nullptr)
	{
		std::cerr << "GunOwner is nullptr, set it properly before calling Shoot function" << std::endl;
		return;
	}

	if (!canShoot)
	{
		return;
	}

	if (GameManager* manager = gunOwner->GetGame())
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		Vec3 mousePos = MMath::inverse(manager->getProjectionMatrix()) * Vec3(float(x), float(y), 0.0f);
		Vec3 direction = VMath::normalize(mousePos - gunOwner->getPos());
		float desiredAngle = std::atan2(direction.y, direction.x);
		float orientation = -desiredAngle;

		float mass = 1.0f;
		float rotation = 0.0f;
		float angular = 0.0f;
		float movementSpeed = 20.0f;
		float scale = 0.5f;
		float lifeTime = 2.f;

		Vec3 size(0.5f, 0.5f, 0.0f);
		Vec3 position = gunOwner->getPos() + direction * 1.2f;
		Vec3 velocity(0.0f, 0.0f, 0.0f);
		Vec3 acceleration(0.0f, 0.0f, 0.0f);

		Bullet* bullet = new Bullet(
			this,
			position,
			velocity,
			acceleration,
			size,
			mass,
			orientation,
			rotation,
			angular,
			movementSpeed,
			scale,
			lifeTime,
			manager);
		bullet->OnCreate();
		bullet->SetDirection(direction);
		manager->getBullets()->push_back(bullet);
		canShoot = false;
		currentAmmo--;
		if (currentAmmo == 0)
		{
			canShoot = false;
			timerId = SDL_AddTimer(reloadSpeed * 1000.f, ReloadTimerCallback, reinterpret_cast<Gun*>(this));
			if (timerId = 0)
			{
				std::cerr << "Error on setting reload timer" << std::endl;
				canShoot = true;
			}
			return;
		}
		// Timer is in ms, while our value is in seconds
		timerId = SDL_AddTimer(fireRate * 1000.f, FireRateTimerCallback, reinterpret_cast<Gun*>(this));
		if (timerId = 0)
		{
			std::cerr << "Error on setting fireRate timer" << std::endl;
			canShoot = true;
			return;
		}
	}
}



Uint32 Gun::FireRateTimerCallback(Uint32 interval, void* param)
{
	Gun* gun = reinterpret_cast<Gun*>(param);
	gun->canShoot = true;
	return 0;
}

Uint32 Gun::ReloadTimerCallback(Uint32 interval, void* param)
{
	Gun* gun = reinterpret_cast<Gun*>(param);
	gun->currentAmmo = gun->maxAmmo;
	gun->canShoot = true;
	return 0;
}

