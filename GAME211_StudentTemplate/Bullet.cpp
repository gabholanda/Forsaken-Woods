#include "Bullet.h"
#include "GameManager.h"

bool Bullet::OnCreate()
{
    if (image) {
        SDL_FreeSurface(image);
        image = nullptr;
    }
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    image = IMG_Load("Bullet.png");
    if (!image) {
        std::cerr << "Can't open the image Bullet.png" << std::endl;
        return false;
    }

    SDL_Renderer* renderer = game->getRenderer();
    texture = SDL_CreateTextureFromSurface(renderer, image);
    if (!texture) {
        std::cerr << "Failed to create bullet texture" << std::endl;
        return false;
    }
    return true;
}

void Bullet::Render()
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

void Bullet::Update(float deltaTime)
{
    pos += direction * movementSpeed * deltaTime;
    lifeTime -= deltaTime;
    if (lifeTime <= 0.0f) {
        setMarkedForDeletion(true);
    }
}
