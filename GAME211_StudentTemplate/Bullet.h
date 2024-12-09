#pragma once
#include "Body.h"
#include <SDL_image.h>
#include <iostream>
#include "VMath.h"

class Gun;
class GameManager;

class Bullet : public Body {
protected:
    GameManager* game;
    Vec3 direction;
    Gun* owningGun;
    float lifeTime;

    SDL_Surface* image;
    SDL_Texture* texture;

public:
    Bullet() : Body(), game(nullptr), owningGun(nullptr), lifeTime(0.0f), image(nullptr), texture(nullptr) {}

    // SetParameters allows re-initialization from the memory pool
    void SetParameters(
        Gun* owningGun_,
        const Vec3& pos_, const Vec3& vel_, const Vec3& accel_,
        const Vec3& size_,
        float mass_,
        float orientation_,
        float rotation_,
        float angular_,
        float movementSpeed_,
        float scale_,
        float lifeTime_,
        GameManager* game_
    ) {
        owningGun = owningGun_;
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
        lifeTime = lifeTime_;
        game = game_;
        direction = Vec3(0.0f, 0.0f, 0.0f);
        markedForDeletion = false;
    }

    Gun* GetOwningGun() { return owningGun; }
    float GetLifeTime() const { return lifeTime; }
    Vec3 GetDirection() const { return direction; }
    void SetDirection(Vec3 direction_) { direction = direction_; }
    bool OnCreate();
    void Render();
    void Update(float deltaTime);
    void setTexture(SDL_Texture* texture_) { texture = texture_; }

    bool getMarkedForDeletion() { return markedForDeletion; }
    void setMarkedForDeletion(bool val) { markedForDeletion = val; }
};
