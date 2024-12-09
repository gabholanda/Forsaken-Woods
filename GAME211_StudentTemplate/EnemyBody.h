#pragma once
#include "Body.h"
#include <SDL_image.h>
#include <iostream>
#include "VMath.h"

class GameManager;
class PlayerBody;
class Gun;

class EnemyBody : public Body {
protected:
    GameManager* game;
    Gun* gun;
    float enemyHp;
    SDL_Surface* image;
    SDL_Texture* texture;

public:
    EnemyBody() : Body(), game(nullptr), gun(nullptr), enemyHp(100.0f), image(nullptr), texture(nullptr) {}

    void SetParameters(
        Gun* gun_,
        const Vec3& pos_, const Vec3& vel_, const Vec3& accel_,
        const Vec3& size_,
        float mass_,
        float orientation_,
        float rotation_,
        float angular_,
        float movementSpeed_,
        float scale_,
        GameManager* game_,
        float enemyHp_
    ) {
        gun = gun_;
        pos = pos_;
        vel = vel_;
        accel = accel_;
        size = size_;
        mass = mass_;
        orientation = orientation_;
        rotation = rotation_;
        angular = angular_;
        movementSpeed = movementSpeed_;
        scale = scale_;
        game = game_;
        enemyHp = enemyHp_;
    }

    ~EnemyBody();

    bool OnCreate();
    void Render();
    void Update(float deltaTime);
    void setTexture(SDL_Texture* texture_) { texture = texture_; }
    void MoveTowardsPlayer(float deltaTime, PlayerBody* target);
    GameManager* GetGame() const { return game; }
    void RangeAttack(PlayerBody* target);
    float getHp() { return enemyHp; }
    void setHp(float enemyHp_) { enemyHp = enemyHp_; }
    Gun* GetGun() const { return gun; }
    void dealDamageForEnemy(float damage, float playerHp);
};
