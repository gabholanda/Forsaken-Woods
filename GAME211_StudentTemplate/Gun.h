#pragma once
#ifndef GUN_H
#define GUN_H


class Gun {
protected:
    float attackSpeed;
    float damage;
    float reloadSpeed;
    int ammo;

public:
    Gun(float attackSpeed_, float damage_, float reloadSpeed_, int ammo_)
        : attackSpeed(attackSpeed_), damage(damage_), reloadSpeed(reloadSpeed_), ammo(ammo_) {}

    virtual void Shoot();
};

#endif