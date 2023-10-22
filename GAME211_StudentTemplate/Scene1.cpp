#include <VMath.h>
#include "Bullet.h"
#include "Collision.h"
#include "EnemyBody.h"
#include "PlayerCamera.h"
#include "Scene1.h"
// See notes about this constructor in Scene1.h.
Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_) {
	window = sdlWindow_;
	game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
	camera = nullptr;
}

Scene1::~Scene1() {
}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	camera = new PlayerCamera(w, h, xAxis, yAxis, game);
	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	SDL_Surface* enemyImage;
	SDL_Texture* enemyTexture;

	enemyImage = IMG_Load("rogue.png");
	enemyTexture = SDL_CreateTextureFromSurface(renderer, enemyImage);
	for (EnemyBody* enemy : game->getEnemies()) {
		enemy->setImage(enemyImage);
		enemy->setTexture(enemyTexture);
	}

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
		enemy->Update(deltaTime);
		if (Collision::CheckCollision(*game->getPlayer(), *enemy))
		{
			std::cout << "Collided" << std::endl;
		}
	}

	for (int i = 0; i < game->getBullets()->size(); i++)
	{
		game->getBullets()->at(i)->Update(deltaTime);
		if (game->getBullets()->at(i)->GetLifeTime() <= 0)
		{
			// avoid destroying accidently erasing wrong bullet
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
	}
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

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
