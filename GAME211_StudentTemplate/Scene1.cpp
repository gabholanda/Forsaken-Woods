#include <VMath.h>
#include "Bullet.h"
#include "Collision.h"
#include "EnemyBody.h"
#include "PlayerCamera.h"
#include "Grid.h"
#include "UIText.h"
#include "Scene1.h"
#include "CollisionTile.h"
#include "DecorationTile.h"

Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_) {
    window = sdlWindow_;
    game = game_;
    renderer = SDL_GetRenderer(window);
    xAxis = 25.0f;
    yAxis = 15.0f;
    camera = nullptr;
    randomizer = nullptr;
}

Scene1::~Scene1() {
}

bool Scene1::OnCreate() {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    camera = new PlayerCamera(w, h, xAxis, yAxis, game);
    buffUITimer = 0.0;
    return true;
}

void Scene1::OnDestroy() {
    delete camera;
    delete randomizer;
    delete renderer;  // If these were created by SDL, consider using SDL_DestroyRenderer, etc.
    delete window;
}

void Scene1::Update(const float deltaTime) {

    if (buffUITimer > 0) {
        buffUITimer -= deltaTime;
    }
    else {
        game->getBuff()->setCanCollect(false);
    }

    camera->updateCameraPosition();
    // Update player
    game->getPlayer()->Update(deltaTime);

    // Update enemies
    for (auto& enemy : *game->getEnemies()) {
        enemy->MoveTowardsPlayer(deltaTime, game->getPlayer());
        enemy->RangeAttack(game->getPlayer());
        enemy->Update(deltaTime);

        // Check collisions with collision tiles
        for (size_t i = 0; i < game->getGrid()->GetCollisionTiles()->size(); i++) {
            if (Collision::CheckCollision(game->getGrid()->GetCollisionTiles()->at(i), *enemy)) {
                Collision::ResolveCollision(enemy, &game->getGrid()->GetCollisionTiles()->at(i));
            }
        }
        // Check collisions with buff bodies
        for (Buff* buffBody : *game->getBuffBodies()) {
            if (Collision::CheckCollision(*buffBody, *enemy)) {
                Collision::ResolveCollision(enemy, buffBody);
            }
        }
    }

    // Check player collisions with collision tiles
    for (size_t i = 0; i < game->getGrid()->GetCollisionTiles()->size(); i++) {
        if (Collision::CheckCollision(*game->getPlayer(), game->getGrid()->GetCollisionTiles()->at(i))) {
            Collision::ResolveCollision(game->getPlayer(), &game->getGrid()->GetCollisionTiles()->at(i));
        }
    }

    // Check player collisions with buff bodies
    for (Buff* buffBody : *game->getBuffBodies()) {
        if (Collision::CheckCollision(*game->getPlayer(), *buffBody)) {
            Collision::ResolveCollision(game->getPlayer(), buffBody);

            irrklang::ISound* sound = game->getSoundEngine()->play2D("Power Up.wav", false, false, true);
            sound->setVolume(1.5f);

            // Apply a random buff
            Buff* tempBuff = game->getBuffManager()->randomBuff();
            tempBuff->ApplyBuff(game->getPlayer());
            delete game->getBuff();   // Delete the old buff
            game->setBuff(tempBuff);

            std::cout << tempBuff->Text() << std::endl;
            buffBody->setMarkedForDeletion(true);
            game->getBuff()->setCanCollect(true);
            buffUITimer = 2.0;
        }
    }

    // Manage bullets
    for (int i = 0; i < game->getBullets()->size(); i++) {
        // If bullet marked for deletion, remove it
        if (game->getBullets()->at(i)->getMarkedForDeletion()) {
            Bullet* bulletToDestroy = game->getBullets()->at(i);
            game->getBullets()->erase(game->getBullets()->begin() + i);
            game->DestroyBullet(bulletToDestroy);
            i--;
           // continue; // Move on to the next bullet
        }

        // Check bullet collisions with buff bodies
        for (Buff* buffBody : *game->getBuffBodies()) {
            if (Collision::CheckCollision(*buffBody, *game->getBullets()->at(i))) {
                game->getBullets()->at(i)->setMarkedForDeletion(true);
            }
        }

        // Check bullet collisions with collision tiles
        for (size_t j = 0; j < game->getGrid()->GetCollisionTiles()->size(); j++) {
            if (Collision::CheckCollision(game->getGrid()->GetCollisionTiles()->at(j), *game->getBullets()->at(i))) {
                game->getBullets()->at(i)->setMarkedForDeletion(true);
            }
        }

        // Check bullet collisions with enemies
        for (auto& enemy : *game->getEnemies()) {
            if (Collision::CheckCollision(*game->getBullets()->at(i), *enemy)) {
                game->getBullets()->at(i)->setMarkedForDeletion(true);

                float enemyHp = enemy->getHp();
                float damage = game->getBullets()->at(i)->GetOwningGun()->GetDamage();
                enemy->setHp(enemyHp - damage);

                if (enemy->getHp() <= 0.0f) {
                    enemy->setMarkedForDeletion(true);
                }

                std::cout << "EnemyHP: " << enemyHp - damage << std::endl;
            }
        }
    }

    // Manage enemy bullets
    for (int i = 0; i < game->getEnemyBullets()->size(); i++) {
        // If enemy bullet marked for deletion, remove it
        if (game->getEnemyBullets()->at(i)->getMarkedForDeletion()) {
            Bullet* bulletToDestroy = game->getEnemyBullets()->at(i);
            game->getEnemyBullets()->erase(game->getEnemyBullets()->begin() + i);
            game->DestroyBullet(bulletToDestroy);
            i--;
            continue;
        }

        // Check enemy bullet collisions with buff bodies
        for (Buff* buffBody : *game->getBuffBodies()) {
            if (Collision::CheckCollision(*buffBody, *game->getEnemyBullets()->at(i))) {
                game->getEnemyBullets()->at(i)->setMarkedForDeletion(true);
            }
        }

        // Check enemy bullet collisions with collision tiles
        for (size_t j = 0; j < game->getGrid()->GetCollisionTiles()->size(); j++) {
            if (Collision::CheckCollision(game->getGrid()->GetCollisionTiles()->at(j), *game->getEnemyBullets()->at(i))) {
                game->getEnemyBullets()->at(i)->setMarkedForDeletion(true);
            }
        }

        // Check enemy bullet collisions with player
        if (Collision::CheckCollision(*game->getEnemyBullets()->at(i), *game->getPlayer())) {
            game->getEnemyBullets()->at(i)->setMarkedForDeletion(true);
            float playerHp = game->getPlayer()->getHp();
            float enemyDamage = game->getEnemyBullets()->at(i)->GetOwningGun()->GetDamage();
            if (enemyDamage > 0) {
                game->getPlayer()->setHp(playerHp - (enemyDamage / 2.5));
            }
            if (game->getPlayer()->getHp() <= 0.0f) {
                game->getPlayer()->Death();
            }
            std::cout << "PlayerHP:" << playerHp << std::endl;
        }
    }
    for (auto& bullet : *game->getBullets()) {
        bullet->Update(deltaTime);
    }
    for (auto& enemyBullet : *game->getEnemyBullets()) {
        enemyBullet->Update(deltaTime);
    }
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	game->StartRenderImGui();
	game->RenderDebugGrid();
	game->RenderTiles();
	game->RenderPlayer();
	game->RenderEnemy();
	game->RenderBullets();
	game->getGrid()->RenderCollisionTiles();
	game->RenderDebug();
	game->RenderBuffBody();
	game->RenderUI();
	if (game->inUi) {
		game->EndRenderImGui();
	}
	SDL_GL_SwapWindow(game->getWindor()->GetSDL_Window()); // Swap OpenGL buffers
}

void Scene1::PostRenderUpdate(const float time)
{
    // Bullets
    for (int i = 0; i < game->getBullets()->size(); i++) {
        if (game->getBullets()->at(i)->getMarkedForDeletion()) {
            Bullet* bulletToDestroy = game->getBullets()->at(i);
            game->getBullets()->erase(game->getBullets()->begin() + i);
            game->DestroyBullet(bulletToDestroy);
            i--;
        }
     
    }

    // Enemy Bullets
    for (int i = 0; i < game->getEnemyBullets()->size(); i++) {
        if (game->getEnemyBullets()->at(i)->getMarkedForDeletion()) {
            Bullet* bulletToDestroy = game->getEnemyBullets()->at(i);
            game->getEnemyBullets()->erase(game->getEnemyBullets()->begin() + i);
            game->DestroyBullet(bulletToDestroy);
            i--;
        }
    }

    // Enemies
    for (int i = 0; i < game->getEnemies()->size(); i++) {
        if (game->getEnemies()->at(i)->getMarkedForDeletion()) {
            EnemyBody* enemyToDestroy = game->getEnemies()->at(i);
            game->getEnemies()->erase(game->getEnemies()->begin() + i);
            game->DestroyEnemy(enemyToDestroy);
            i--;
        }
    }

    // BuffBodies (no memory pool here yet, so we can still delete)
    for (int i = 0; i < game->getBuffBodies()->size(); i++) {
        if (game->getBuffBodies()->at(i)->getMarkedForDeletion()) {
            delete game->getBuffBodies()->at(i);
            game->getBuffBodies()->erase(game->getBuffBodies()->begin() + i);
            i--;
        }
    }

    if (game->getEnemies()->empty()) {
        irrklang::ISound* sound = game->getSoundEngine()->play2D("Win Level.wav", false, false, true);
        sound->setVolume(2.0f);
        game->OnWin();
    }
}

void Scene1::HandleEvents(const SDL_Event& event) {
    // send events to player as needed
    game->getPlayer()->HandleEvents(event);
}
