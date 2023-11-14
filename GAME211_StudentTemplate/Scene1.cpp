#include <VMath.h>
#include "Bullet.h"
#include "Collision.h"
#include "EnemyBody.h"
#include "PlayerCamera.h"
#include "Grid.h"
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
	for (EnemyBody* enemy : game->getEnemies()) {
		enemy->setImage(enemyImage);
		enemy->setTexture(enemyTexture);
	}
	Tile* exampleTile = new Tile(Vec3(10, 10, 0), 0.0f, 1.f,
		game->getBackgroundSpritesheetReader()->GetRows(),
		game->getBackgroundSpritesheetReader()->GetColumns(),
		game->getBackgroundSpritesheetReader()->GetRects()[0][0],
		game);
	exampleTile->setImage(game->getBackgroundSpritesheetReader()->GetImage());
	exampleTile->setTexture(game->getBackgroundSpritesheetReader()->GetTexture());
	game->getGrid()->PushTile(exampleTile, 0);
	return true;
}

void Scene1::OnDestroy()
{
	delete camera;
}

void Scene1::Update(const float deltaTime) {

	camera->updateCameraPosition();
	// Update player
	game->getPlayer()->Update(deltaTime);
	for (auto& enemy : game->getEnemies()) {
		enemy->MoveTowardsPlayer(deltaTime, game->getPlayer());
		enemy->RangeAttack(game->getPlayer());
		enemy->Update(deltaTime);
		if (Collision::CheckCollision(*game->getPlayer(), *enemy))
		{
			std::cout << "Collided" << std::endl;
			game->getBuffManager()->PickRandomBuff();

		}
	}

	if (Collision::CheckCollision(*game->getPlayer(), *game->getBuffManager()->GetBuffs()[0])) {
		game->getBuffManager()->GetBuffs()[0]->ApplyBuff(game->getPlayer());
	}

	for (int i = 0; i < game->getBullets()->size(); i++)
	{
		game->getBullets()->at(i)->Update(deltaTime);
		if (game->getBullets()->at(i)->GetLifeTime() <= 0)
		{
			// avoid destroying accidently wrong bullet
			bulletsToDestroy.push_back(i);
			return;
		}

		for (auto& enemy : game->getEnemies()) {
			if (Collision::CheckCollision(*game->getBullets()->at(i), *enemy))
			{
				// Push bullets to deletion pool
				bulletsToDestroy.push_back(i);
				// Do damage here
			}
		}

		if (Collision::CheckCollision(*game->getBullets()->at(i), *game->getPlayer())) {
			// Handle bullet-player collision
			bulletsToDestroy.push_back(i);
			// Do damage to player here
		}
	}



}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	game->RenderTiles();
	game->RenderPlayer();
	game->RenderEnemy();
	game->RenderBullets();
	game->RenderDebug();
	SDL_RenderPresent(renderer);
}

void Scene1::PostRenderUpdate(const float time)
{
	for (int i = 0; i < bulletsToDestroy.size(); i++)
	{
		game->getBullets()->erase(game->getBullets()->begin() + bulletsToDestroy[i]);
	}

	bulletsToDestroy.clear();
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
}
