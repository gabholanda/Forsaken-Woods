#include "Scene2.h"
#include <VMath.h>
#include "PlayerCamera.h"
#include "EnemyBody.h"
#include "Collision.h"
// See notes about this constructor in Scene1.h.
Scene2::Scene2(SDL_Window* sdlWindow_, GameManager* game_) {
	window = sdlWindow_;
	game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
	camera = nullptr;
}

Scene2::~Scene2() {
}

bool Scene2::OnCreate() {
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
	for (EnemyBody* enemy : game->getEnemies()) {
		enemy->setImage(enemyImage);
		enemy->setTexture(enemyTexture);
	}


	return true;
}

void Scene2::OnDestroy() {}

void Scene2::Update(const float deltaTime) {

	camera->updateCameraPosition();
	// Update player
	game->getPlayer()->Update(deltaTime);
	for (auto& enemy : game->getEnemies()) {
		enemy->Update(deltaTime);
		if (Collision::CheckCollision(*game->getPlayer(), *enemy))
		{
			std::cout << "Collided" << std::endl;
		}
	}

	Vec3 origin(0.0f, 0.0f, 0.0f);
	if (VMath::distance(game->getPlayer()->getPos(), origin) < 1.5f)
	{
		SDL_Event event;
		SDL_memset(&event, 0, sizeof(event));
		event.type = game->getChangeScene();
		event.user.code = 1;
		event.user.data1 = nullptr;
		event.user.data2 = nullptr;
		SDL_PushEvent(&event);
	}

}

void Scene2::Render() {
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

void Scene2::PostRenderUpdate(const float time)
{
}

void Scene2::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
}
