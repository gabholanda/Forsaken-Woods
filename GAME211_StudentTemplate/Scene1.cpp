#include <VMath.h>
#include "Bullet.h"
#include "Collision.h"
#include "EnemyBody.h"
#include "PlayerCamera.h"
#include "Grid.h"
#include "CollisionTile.h"
#include "Scene1.h"
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
	SDL_Surface* enemyImage;
	SDL_Texture* enemyTexture;

	enemyImage = IMG_Load("treantenemy.png");
	enemyTexture = SDL_CreateTextureFromSurface(renderer, enemyImage);
	for (EnemyBody* enemy : *game->getEnemies()) {
		enemy->setImage(enemyImage);
		enemy->setTexture(enemyTexture);
	}
	Tile* exampleTile = new Tile(Vec3(10, 10, 0), 0.0f, 1.f,
		game->getBackgroundSpritesheetReader()->GetRows(),
		game->getBackgroundSpritesheetReader()->GetColumns(),
		// This defines which section of the spritesheet we gonna get
		game->getBackgroundSpritesheetReader()->GetRects()[0][0],
		game);
	exampleTile->setImage(game->getBackgroundSpritesheetReader()->GetImage());
	exampleTile->setTexture(game->getBackgroundSpritesheetReader()->GetTexture());


	CollisionTile* exampleCollisionTile = new CollisionTile(Vec3(10, 10, 0), 0.0f, 1.f,
		game->getBackgroundSpritesheetReader()->GetRows(),
		game->getBackgroundSpritesheetReader()->GetColumns(),
		Vec3(4.0f, 4.0f, 0.0f),
		// This defines which section of the spritesheet we gonna get
		game->getBackgroundSpritesheetReader()->GetRects()[0][3],
		game);
	exampleCollisionTile->Tile::setImage(game->getBackgroundSpritesheetReader()->GetImage());
	exampleCollisionTile->Tile::setTexture(game->getBackgroundSpritesheetReader()->GetTexture());
	//game->getGrid()->PushTile(exampleCollisionTile, 0);

	// Setting grid borders
	/*TODO: Change this to dynamicly fill the tiles*/

	// Down
	for (size_t i = 0; i < 15; i++)
	{
		game->getGrid()->PushTile(exampleCollisionTile, i);
	}

	// Left
	for (size_t i = 15; i < 211; i += 15)
	{
		game->getGrid()->PushTile(exampleCollisionTile, i);
	}

	// Up
	for (size_t i = 211; i < 225; i++)
	{
		game->getGrid()->PushTile(exampleCollisionTile, i);
	}

	// Right
	for (size_t i = 14; i < 224; i += 15)
	{
		game->getGrid()->PushTile(exampleCollisionTile, i);
	}

	// Set up arena
	for (size_t i = 16; i < 210; i++)
	{
		game->getGrid()->PushTile(exampleTile, i);
		if (i % 15 == 13)
		{
			i += 2;
		}
	}


	// Test tile
	game->getGrid()->PushTile(exampleCollisionTile, 34);
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

	if (Collision::CheckCollision(*game->getPlayer(), *game->getBuffManager()->GetBuffs()[0])) {
		game->getBuffManager()->GetBuffs()[game->getBuffManager()->PickRandomBuffIndex()]->ApplyBuff(game->getPlayer());
	}

	for (int i = 0; i < game->getBullets()->size(); i++)
	{
		game->getBullets()->at(i)->Update(deltaTime);
		if (game->getBullets()->at(i)->GetLifeTime() <= 0)
		{
			game->getBullets()->at(i)->setMarkedForDeletion(true);
			return;
		}

		for (size_t j = 0; j < game->getGrid()->GetCollisionTiles()->size(); j++)
		{
			if (Collision::CheckCollision(game->getGrid()->GetCollisionTiles()->at(j), *game->getBullets()->at(i)))
			{
				game->getBullets()->at(i)->setMarkedForDeletion(true);
				return;
			}
		}

		for (auto& enemy : *game->getEnemies()) {
			if (Collision::CheckCollision(*game->getBullets()->at(i), *enemy))
			{
				// Push bullets to deletion 
				game->getBullets()->at(i)->setMarkedForDeletion(true);
				float enemyHp = enemy->getHp();
				float damage = game->getBullets()->at(i)->GetOwningGun()->GetDamage();
				enemy->setHp(enemyHp - damage);
				if (enemyHp <= 0)
				{
					enemy->setMarkedForDeletion(true);
					return;
				}
				std::cout << "EnemyHP: " << enemyHp << std::endl;
				return;
			}
			return;
		}
	}

	for (int i = 0; i < game->getEnemyBullets()->size(); i++)
	{
		game->getEnemyBullets()->at(i)->Update(deltaTime);
		if (game->getEnemyBullets()->at(i)->GetLifeTime() <= 0)
		{
			game->getEnemyBullets()->at(i)->setMarkedForDeletion(true);
			return;
		}


		for (size_t j = 0; j < game->getGrid()->GetCollisionTiles()->size(); j++)
		{
			if (Collision::CheckCollision(game->getGrid()->GetCollisionTiles()->at(j), *game->getEnemyBullets()->at(i)))
			{
				game->getEnemyBullets()->at(i)->setMarkedForDeletion(true);
				return;
			}
		}

		if (Collision::CheckCollision(*game->getEnemyBullets()->at(i), *game->getPlayer()))
		{
			game->getEnemyBullets()->at(i)->setMarkedForDeletion(true);
			float playerHp = game->getPlayer()->getHp();
			float enemyDamage = game->getEnemyBullets()->at(i)->GetOwninGun()->GetDamage();
			game->getPlayer()->setHp(playerHp - enemyDamage);
			if (playerHp <= 0)
			{
				game->getPlayer()->Death();
				return;
			}
			std::cout << "PlayerHP:" << playerHp << std::endl;
			return;
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
	SDL_RenderPresent(renderer);
}

void Scene1::PostRenderUpdate(const float time)
{
	for (int i = 0; i < game->getBullets()->size(); i++)
	{
		if (game->getBullets()->at(i)->getMarkedForDeletion())
		{
			game->getBullets()->erase(game->getBullets()->begin() + i);
			i--;
		}
	}

	for (int i = 0; i < game->getEnemyBullets()->size(); i++)
	{
		if (game->getEnemyBullets()->at(i)->getMarkedForDeletion())
		{
			game->getEnemyBullets()->erase(game->getEnemyBullets()->begin() + i);
			i--;
		}
	}

	for (int i = 0; i < game->getEnemies()->size(); i++)
	{
		if (game->getEnemies()->at(i)->getMarkedForDeletion())
		{
			game->getEnemies()->erase(game->getEnemies()->begin() + i);
			i--;
		}
	}
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
}
