#include "Gun.h"
#include "Bullet.h"
#include "PlayerBody.h"
#include "EnemyBody.h"
#include "GameManager.h"
#include <MMath.h>
#include <VMath.h>
#include <irrKlang.h>
#include <cmath>
#include <iostream>

#define VERY_SMALL 0.0001f

Gun::~Gun() {
    // Cleanup if needed
}

void Gun::ShootForEnemy(PlayerBody* target)
{
    if (gunOwnerEnemy == nullptr) {
        std::cerr << "GunOwnerEnemy is nullptr, set it before calling ShootForEnemy." << std::endl;
        return;
    }

    if (!canShoot) return;

    if (GameManager* manager = gunOwnerEnemy->GetGame()) {
        if (VMath::distance(target->getPos(), gunOwnerEnemy->getPos()) <= VERY_SMALL) {
            return;
        }
        Vec3 direction = VMath::normalize(target->getPos() - gunOwnerEnemy->getPos());
        float desiredAngle = std::atan2(direction.y, direction.x);
        float orientation = -desiredAngle;

        float mass = 1.0f;
        float rotation = 0.0f;
        float angular = 0.0f;
        float movementSpeed = 20.0f;
        float scale = 0.5f;
        float lifetime = 2.0f;

        Vec3 size(0.5f, 0.5f, 0.0f);
        Vec3 position = gunOwnerEnemy->getPos() + direction * 1.2f;
        Vec3 velocity(0.0f, 0.0f, 0.0f);
        Vec3 acceleration(0.0f, 0.0f, 0.0f);

        // Use the pool through the GameManager
        Bullet* enemyBullet = manager->CreateBullet(
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
            lifetime
        );
        enemyBullet->SetDirection(direction);
        enemyBullet->setMarkedForDeletion(false);
        manager->getEnemyBullets()->push_back(enemyBullet);

        canShoot = false;
        timerId = SDL_AddTimer((Uint32)(fireRate * 1000.f), FireRateTimerCallback, this);
        if (timerId == 0) {
            std::cerr << "Error setting fireRate timer" << std::endl;
            canShoot = true;
        }
    }
}

void Gun::Shoot()
{
    if (gunOwner == nullptr) {
        std::cerr << "GunOwner is nullptr, set it before calling Shoot." << std::endl;
        return;
    }

    if (!canShoot) return;

    if (GameManager* manager = gunOwner->GetGame()) {
        irrklang::ISound* sound = manager->getSoundEngine()->play2D("Shoot Sound Effect.wav", false, false, true);
        sound->setVolume(0.8f);

        int x, y;
        SDL_GetMouseState(&x, &y);
        Vec3 mousePos = MMath::inverse(manager->getProjectionMatrix()) * Vec3((float)x, (float)y, 0.0f);
        Vec3 direction = VMath::normalize(mousePos - gunOwner->getPos());
        float desiredAngle = std::atan2(direction.y, direction.x);
        float orientation = -desiredAngle;

        float mass = 1.0f;
        float rotation = 0.0f;
        float angular = 0.0f;
        float movementSpeed = 20.0f;
        float scale = 0.5f;
        float lifetime = 2.0f;

        Vec3 size(0.5f, 0.5f, 0.0f);
        Vec3 position = gunOwner->getPos() + direction * 1.2f;
        Vec3 velocity(0.0f, 0.0f, 0.0f);
        Vec3 acceleration(0.0f, 0.0f, 0.0f);

        // Use the pool through the GameManager
        Bullet* bullet = manager->CreateBullet(
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
            lifetime
        );
        bullet->SetDirection(direction);
        bullet->setMarkedForDeletion(false);
        manager->getBullets()->push_back(bullet);
        canShoot = false;
        currentAmmo--;

        timerId = SDL_AddTimer((Uint32)(fireRate * 1000.f), FireRateTimerCallback, this);
        if (timerId == 0) {
            std::cerr << "Error on setting fireRate timer" << std::endl;
            canShoot = true;
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
    gun->isReloading = false;
    gun->canShoot = true;
    return 0;
}

void Gun::SaveState() {
    gunBuffedStats.fireRate = fireRate;
    gunBuffedStats.damage = damage;
    gunBuffedStats.reloadSpeed = reloadSpeed;
}

void Gun::SaveAdditionalStats() {
    gunAdditionalStats.fireRate = gunBuffedStats.fireRate - gunInitialStats.fireRate;
    gunAdditionalStats.damage = gunBuffedStats.damage - gunInitialStats.damage;
    gunAdditionalStats.reloadSpeed = gunBuffedStats.reloadSpeed - gunInitialStats.reloadSpeed;
}

void Gun::ApplyAdditionalStats(Gun* gun)
{
    fireRate += gun->gunAdditionalStats.fireRate;
    damage += gun->gunAdditionalStats.damage;
    reloadSpeed += gun->gunAdditionalStats.reloadSpeed;
}

void Gun::SaveInitialStats()
{
    gunInitialStats.fireRate = fireRate;
    gunInitialStats.damage = damage;
    gunInitialStats.reloadSpeed = reloadSpeed;
}

void Gun::Reload()
{
    isReloading = true;
    canShoot = false;
    timerId = SDL_AddTimer((Uint32)(reloadSpeed * 1000.f), ReloadTimerCallback, this);
    if (timerId == 0) {
        std::cerr << "Error on setting reload timer" << std::endl;
        isReloading = false;
        canShoot = true;
    }
}
