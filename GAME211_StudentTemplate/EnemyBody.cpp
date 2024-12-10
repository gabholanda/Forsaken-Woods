#include "EnemyBody.h"
#include "RotationUtils.h"
#include "Gun.h"
#include "GameManager.h"
#include "PlayerBody.h"

#define VERY_SMALL 0.0001f

EnemyBody::~EnemyBody()
{
    if (image) {
        SDL_FreeSurface(image);
        image = nullptr;
    }
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    delete gun;
}

bool EnemyBody::OnCreate()
{
    if (image) {
        SDL_FreeSurface(image);
        image = nullptr;
    }
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    image = IMG_Load("TreantEnemy.png");
    if (!image) {
        std::cerr << "Can't open the image TreantEnemy.png" << std::endl;
        return false;
    }
    SDL_Renderer* renderer = game->getRenderer();
    texture = SDL_CreateTextureFromSurface(renderer, image);
    if (!texture) {
        std::cerr << "Failed to create enemy texture" << std::endl;
        return false;
    }
    return true;
}


void EnemyBody::Render()
{
    SDL_Renderer* renderer = game->getRenderer();
    Matrix4 projectionMatrix = game->getProjectionMatrix();

    SDL_Rect square;
    Vec3 screenCoords = projectionMatrix * pos;
    float w = image->w * scale;
    float h = image->h * scale;

    square.x = static_cast<int>(screenCoords.x - 0.5f * w);
    square.y = static_cast<int>(screenCoords.y - 0.5f * h);
    square.w = static_cast<int>(w);
    square.h = static_cast<int>(h);

    float orientationDegrees = orientation * 180.0f / M_PI;
    SDL_RenderCopyEx(renderer, texture, nullptr, &square,
        orientationDegrees, nullptr, SDL_FLIP_NONE);
}

void EnemyBody::Update(float deltaTime)
{
    Body::Update(deltaTime);
}

void EnemyBody::MoveTowardsPlayer(float deltaTime, PlayerBody* target)
{
    float distanceToPlayer = VMath::distance(pos, target->getPos());
    if (distanceToPlayer <= 15.0f) {
        RotationUtils::RotateTowardsTarget(orientation, target->getPos(), pos);
        if (VMath::distance(target->getPos(), pos) < VERY_SMALL) {
            return;
        }
        Vec3 direction = VMath::normalize(target->getPos() - pos);
        pos.x += direction.x * movementSpeed * deltaTime;
        pos.y += direction.y * movementSpeed * deltaTime;
    }
}

void EnemyBody::RangeAttack(PlayerBody* target)
{
    if (gun) {
        if (VMath::distance(pos, target->getPos()) <= 10) {
            gun->ShootForEnemy(target);
        }
    }
}

void EnemyBody::dealDamageForEnemy(float damage, float playerHp)
{
    playerHp -= damage;
}
