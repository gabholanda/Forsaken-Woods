#pragma once
#include "SDL.h"
#include <string>

class PlayerBody;
class EnemyBody;
class GameManager;
class Gun {
protected:
    const char* name;
    float fireRate;
    float damage;
    float reloadSpeed;
    bool canShoot;
    bool isReloading;
    int currentAmmo;
    int maxAmmo;
    PlayerBody* gunOwner;
    EnemyBody* gunOwnerEnemy;
    SDL_TimerID timerId;

    struct GunBuffedStats {
        float fireRate;
        float damage;
        float reloadSpeed;
    } gunBuffedStats;

    struct GunInitialStats {
        float fireRate;
        float damage;
        float reloadSpeed;
    } gunInitialStats;

    struct GunAdditionalStats {
        float fireRate;
        float damage;
        float reloadSpeed;
    } gunAdditionalStats;

public:
    Gun(const char* name_, float fireRate_, float damage_, float reloadSpeed_, int ammo_)
        : name(name_), fireRate(fireRate_), damage(damage_), reloadSpeed(reloadSpeed_), maxAmmo(ammo_)
    {
        canShoot = true;
        isReloading = false;
        gunOwner = nullptr;
        gunOwnerEnemy = nullptr;
        currentAmmo = maxAmmo;
        timerId = 0;
    }

    ~Gun();

    void SetGunOwner(PlayerBody* gunOwner_) { gunOwner = gunOwner_; }
    void SetEnemyGunOwner(EnemyBody* gunOwnerEnemy_) { gunOwnerEnemy = gunOwnerEnemy_; }

    virtual void ShootForEnemy(PlayerBody* target);
    virtual void Shoot();

    static Uint32 FireRateTimerCallback(Uint32 interval, void* param);
    static Uint32 ReloadTimerCallback(Uint32 interval, void* param);

    float GetFireRate() const { return fireRate; }
    int GetMaxAmmo() const { return maxAmmo; }
    float GetReloadSpeed() const { return reloadSpeed; }
    float GetDamage() const { return damage; }
    int getCurrentAmmo() const { return currentAmmo; }
    bool getIsReloading() const { return isReloading; }

    void SetFireRate(float fireRate_) { fireRate = fireRate_; }
    void SetMaxAmmo(int maxAmmo_) { maxAmmo = maxAmmo_; }
    void SetReloadSpeed(float reloadSpeed_) { reloadSpeed = reloadSpeed_; }
    void SetDamage(float damage_) { damage = damage_; }

    const char* GetName() const { return name; }
    const char* Text() const {
        if (currentAmmo == 0 || isReloading) {
            return "Reloading...";
        }
        std::string nameString = std::string(name);
        nameString += " (" + std::to_string(currentAmmo) + " / " + std::to_string(maxAmmo) + ")";
        char* result = new char[nameString.length() + 1];
        strcpy_s(result, nameString.length() + 1, nameString.c_str());
        return result;
    }

    void SaveState();
    void SaveAdditionalStats();
    void ApplyAdditionalStats(Gun* gun);
    void SaveInitialStats();
    void Reload();
};
