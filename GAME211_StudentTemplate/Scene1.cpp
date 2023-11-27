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

// See notes about this constructor in Scene1.h.
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

	return true;
}

void Scene1::OnDestroy()
{
	delete camera;
	delete randomizer;
	delete renderer;
	delete window;
}

void Scene1::Update(const float deltaTime) {

	camera->updateCameraPosition();
	// Update player
	game->getPlayer()->Update(deltaTime);
	for (auto& enemy : *game->getEnemies()) {
		enemy->MoveTowardsPlayer(deltaTime, game->getPlayer());
		enemy->RangeAttack(game->getPlayer());
		enemy->Update(deltaTime);
		for (size_t i = 0; i < game->getGrid()->GetCollisionTiles()->size(); i++)
		{
			if (Collision::CheckCollision(game->getGrid()->GetCollisionTiles()->at(i), *enemy))
			{
				Collision::ResolveCollision(enemy, &game->getGrid()->GetCollisionTiles()->at(i));
			}
		}
	}

	for (size_t i = 0; i < game->getGrid()->GetCollisionTiles()->size(); i++)
	{
		if (Collision::CheckCollision(*game->getPlayer(), game->getGrid()->GetCollisionTiles()->at(i)))
		{
			Collision::ResolveCollision(game->getPlayer(), &game->getGrid()->GetCollisionTiles()->at(i));
		}
	}

	for (Buff* buffBody : *game->getBuffBodies()) {
		if (Collision::CheckCollision(*game->getPlayer(), *buffBody)) {
			Collision::ResolveCollision(game->getPlayer(), buffBody);
			game->getBuffManager()->GetBuffs()[game->getBuffManager()->PickRandomBuffIndex()]->ApplyBuff(game->getPlayer());
		std:cout << game->getBuffManager()->PickRandomBuffIndex() << std::endl;
			buffBody->setMarkedForDeletion(true);
		}
	}

	for (int i = 0; i < game->getBullets()->size(); i++)
	{
		game->getBullets()->at(i)->Update(deltaTime);
		if (game->getBullets()->at(i)->GetLifeTime() <= 0)
		{
			game->getBullets()->at(i)->setMarkedForDeletion(true);
		}

		for (size_t j = 0; j < game->getGrid()->GetCollisionTiles()->size(); j++)
		{
			if (Collision::CheckCollision(game->getGrid()->GetCollisionTiles()->at(j), *game->getBullets()->at(i)))
			{
				game->getBullets()->at(i)->setMarkedForDeletion(true);
			}
		}

		for (auto& enemy : *game->getEnemies())
		{
			if (Collision::CheckCollision(*game->getBullets()->at(i), *enemy))
			{
				// Push bullets to deletion 
				game->getBullets()->at(i)->setMarkedForDeletion(true);
				float enemyHp = enemy->getHp();
				float damage = game->getBullets()->at(i)->GetOwningGun()->GetDamage();
				enemy->setHp(enemyHp - damage);
				if (enemy->getHp() <= 0.0f)
				{
					enemy->setMarkedForDeletion(true);
				}
				std::cout << "EnemyHP: " << enemyHp - damage << std::endl;
			}
		}
	}

	for (int i = 0; i < game->getEnemyBullets()->size(); i++)
	{
		game->getEnemyBullets()->at(i)->Update(deltaTime);
		if (game->getEnemyBullets()->at(i)->GetLifeTime() <= 0)
		{
			game->getEnemyBullets()->at(i)->setMarkedForDeletion(true);
		}


		for (size_t j = 0; j < game->getGrid()->GetCollisionTiles()->size(); j++)
		{
			if (Collision::CheckCollision(game->getGrid()->GetCollisionTiles()->at(j), *game->getEnemyBullets()->at(i)))
			{
				game->getEnemyBullets()->at(i)->setMarkedForDeletion(true);
			}
		}

		if (Collision::CheckCollision(*game->getEnemyBullets()->at(i), *game->getPlayer()))
		{
			game->getEnemyBullets()->at(i)->setMarkedForDeletion(true);
			float playerHp = game->getPlayer()->getHp();
			float enemyDamage = game->getEnemyBullets()->at(i)->GetOwningGun()->GetDamage();
			game->getPlayer()->setHp(playerHp - enemyDamage);
			if (game->getPlayer()->getHp() <= 0.0f)
			{
				game->getPlayer()->Death();
			}
			std::cout << "PlayerHP:" << playerHp << std::endl;
		}

	}
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	game->RenderDebugGrid();
	game->RenderTiles();
	game->RenderPlayer();
	game->RenderEnemy();
	game->RenderBullets();
	game->RenderDebug();
	game->RenderBuffBody();
	game->RenderUI();
	SDL_RenderPresent(renderer);
}

void Scene1::PostRenderUpdate(const float time)
{
	for (int i = 0; i < game->getBullets()->size(); i++)
	{
		if (game->getBullets()->at(i)->getMarkedForDeletion())
		{
			delete game->getBullets()->at(i);
			game->getBullets()->erase(game->getBullets()->begin() + i);
			i--;
		}
	}

	for (int i = 0; i < game->getEnemyBullets()->size(); i++)
	{
		if (game->getEnemyBullets()->at(i)->getMarkedForDeletion())
		{
			delete game->getEnemyBullets()->at(i);
			game->getEnemyBullets()->erase(game->getEnemyBullets()->begin() + i);
			i--;
		}
	}

	for (int i = 0; i < game->getEnemies()->size(); i++)
	{
		if (game->getEnemies()->at(i)->getMarkedForDeletion())
		{
			delete game->getEnemies()->at(i);
			game->getEnemies()->erase(game->getEnemies()->begin() + i);
			i--;
		}
	}

	for (int i = 0; i < game->getBuffBodies()->size(); i++)
	{
		if (game->getBuffBodies()->at(i)->getMarkedForDeletion())
		{
			delete game->getBuffBodies()->at(i);
			game->getBuffBodies()->erase(game->getBuffBodies()->begin() + i);
			i--;
		}
	}

	if (game->getEnemies()->size() == 0)
	{
		game->OnRestart();
	}
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
}
