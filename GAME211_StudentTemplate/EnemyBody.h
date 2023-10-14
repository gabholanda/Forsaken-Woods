#ifndef ENEMYBODY_H
#define ENEMYBODY_H

#pragma once
#include <stdio.h>
#include "Body.h"
#include "GameManager.h"
class EnemyBody : public Body
{
protected:
    class GameManager* game;

public:
    EnemyBody() : Body{}
    {
        game = nullptr;
    }

    // Note the last parameter in this constructor!
    // Look in GameManager.cpp to see how this is called.
    // Look in .cpp file, at Render(), to see why game_ is a parameter.
    EnemyBody(
        Vec3 pos_, Vec3 vel_, Vec3 accel_,
        float mass_,
        float radius_,
        float orientation_,
        float rotation_,
        float angular_,
        float movementSpeed_,
        GameManager* game_
    ) : Body{
          pos_
        , vel_
        , accel_
        , mass_
        , radius_
        , orientation_
        , rotation_
        , angular_
        , movementSpeed_
    }
        , game{ game_ }
    {}

    // use the base class versions of getters
    void setPosition(Vec3 newPos) {
        pos = newPos;
    }

    bool OnCreate();
    void Render(float scale = 1.0f);
    void Update(float deltaTime);
    void setTexture(SDL_Texture* texture_) { texture = texture_; }
    void moveTowardsPlayer();

};

#endif