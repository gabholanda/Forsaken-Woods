#include "Scene3.h"
#include <VMath.h>
#include "PlayerCamera.h"
#include "EnemyBody.h"
#include "Collision.h"
// See notes about this constructor in Scene1.h.
Scene3::Scene3(SDL_Window* sdlWindow_, GameManager* game_) {
	window = sdlWindow_;
	game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
	camera = nullptr;
}

Scene3::~Scene3() {
}

bool Scene3::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	camera = new PlayerCamera(w, h, xAxis, yAxis, game);
	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	// Set player image to PacMan

	SDL_Surface* playerImage;
	SDL_Texture* playerTexture;
	playerImage = IMG_Load("rogue.png");
	playerTexture = SDL_CreateTextureFromSurface(renderer, playerImage);
	game->getPlayer()->setImage(playerImage);
	game->getPlayer()->setTexture(playerTexture);


	SDL_Surface* enemyImage;
	SDL_Texture* enemyTexture;

	enemyImage = IMG_Load("rogue.png");
	enemyTexture = SDL_CreateTextureFromSurface(renderer, enemyImage);
	for (EnemyBody* enemy : *game->getEnemies()) {
		enemy->setImage(enemyImage);
		enemy->setTexture(enemyTexture);
	}


	return true;
}

void Scene3::OnDestroy() {}

void Scene3::Update(const float deltaTime) {

	camera->updateCameraPosition();
	// Update player
	game->getPlayer()->Update(deltaTime);
	for (auto& enemy : *game->getEnemies()) {
		enemy->Update(deltaTime);
		if (Collision::CheckCollision(*game->getPlayer(), *enemy))
		{
			std::cout << "Collided" << std::endl;
		}
	}

}

void Scene3::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	// Testing camera
	//SDL_Surface* image;
	//SDL_Texture* texture;
	//SDL_Rect square;
	//Vec3 screenCoords;
	//screenCoords = camera->getProjectionMatrix() * Vec3(xAxis / 2, yAxis / 2, 0);
	//image = IMG_Load("pacman.png");
	//texture = SDL_CreateTextureFromSurface(renderer, image);
	//float w = image->w * 1;
	//float h = image->h * 1;

	//square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	//square.y = static_cast<int>(screenCoords.y - 0.5f * h);
	//square.w = static_cast<int>(w);
	//square.h = static_cast<int>(h);
	//SDL_RenderCopyEx(renderer, texture, nullptr, &square,
	//	0, nullptr, SDL_FLIP_NONE);
	// render the player
	game->RenderPlayer();
	game->RenderEnemy();
	game->RenderDebug();
	SDL_RenderPresent(renderer);
}

void Scene3::PostRenderUpdate(const float time)
{
}

void Scene3::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
}
