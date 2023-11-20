#include "GameManager.h"
#include "Scene1.h"
#include "EnemyBody.h"
#include "Bullet.h"
#include "EnemyBullet.h"
#include "Collision.h"
#include "Grid.h"

GameManager::GameManager() {
	windowPtr = nullptr;
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
	player = nullptr;
	isDebugging = false;
	backgroundReader = nullptr;

	for (auto enemy : enemies) {
		enemy = nullptr;
	}

	for (Bullet* bullet : bullets) {
		bullet = nullptr;
	}

}

bool GameManager::OnCreate() {
	// My display is 1920 x 1080 but the following seems to work best to fill the screen.
   /* const int SCREEN_WIDTH = 1540;
	const int SCREEN_HEIGHT = 860;*/

	// Use 1000x600 for less than full screen
	const int SCREEN_WIDTH = 1000;
	const int SCREEN_HEIGHT = 600;
	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);
	windowPtr = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);

	if (windowPtr == nullptr) {
		OnDestroy();
		return false;
	}
	if (windowPtr->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		OnDestroy();
		return false;
	}

	// select scene for specific assignment

	currentScene = new Scene1(windowPtr->GetSDL_Window(), this);

	/* Grid needs to be same dimension as our sprites */
	grid = new Grid(160, 160, 15, 15, this);

	// create player
	float mass = 1.0f;
	float orientation = 0.0f;
	float rotation = 0.0f;
	float angular = 0.0f;
	float movementSpeed = 5.0f;

	Gun* gun = Randomizer::getRandomWeapon();
	Gun* randomEnemyGun = Randomizer::getRandomWeapon();

	float scale = 0.5;
	Vec3 size(3.f, 3.f, 0.0f);
	Vec3 position(0.5f * currentScene->getxAxis(), 0.5f * currentScene->getyAxis(), 0.0f);
	//Vec3 position(0.0f, 0.0f, 0.0f);
	Vec3 velocity(0.0f, 0.0f, 0.0f);
	Vec3 acceleration(0.0f, 0.0f, 0.0f);

	player = new PlayerBody
	(
		gun,
		position,
		velocity,
		acceleration,
		size,
		mass,
		orientation,
		rotation,
		angular,
		movementSpeed,
		scale,
		this
	);

	gun->SetGunOwner(player);

	buffManager = new BuffManager();
	for (int i = 0; i < buffManager->GetBuffs().size(); i++) {
		buffManager->GetBuffs()[i] = new Buff(
			position,
			velocity,
			acceleration,
			size,
			mass,
			orientation,
			rotation,
			angular,
			movementSpeed,
			scale,
			this);
	}

	for (int i = 0; i < 1; i++) {
		float massEnemy = 1.0f;
		float orientationEnemy = 0.0f;
		float rotationEnemy = 0.0f;
		float angularEnemy = 0.0f;
		float movementSpeedEnemy = 1.0f;
		float scaleEnemy = 0.5;
		Vec3 sizeEnemy(3.f, 3.f, 0.0f);
		Vec3 positionEnemy(0.3f * currentScene->getxAxis(), 0.3f * currentScene->getyAxis(), 0.0f);
		Vec3 velocityEnemy(0.0f, 0.0f, 0.0f);
		Vec3 accelerationEnemy(0.0f, 0.0f, 0.0f);

		EnemyBody* newEnemy = new EnemyBody(
			randomEnemyGun,
			positionEnemy,
			velocityEnemy,
			accelerationEnemy,
			sizeEnemy,
			massEnemy,
			orientationEnemy,
			rotationEnemy,
			angularEnemy,
			movementSpeedEnemy,
			scaleEnemy,
			this
		);
		randomEnemyGun->SetEnemyGunOwner(newEnemy);
		enemies.push_back(newEnemy);
	}


	if (player->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	for (auto& currentEnemy : enemies) {
		if (currentEnemy->OnCreate() == false) {
			OnDestroy();
			return false;
		}
	}

	// the image is actually 1120x640, 7 rows of 160 and 7 columns of 4
	backgroundReader = new SpritesheetReader(160, 160, 7, 4);
	backgroundReader->LoadFromFile("Ground Tiles_16x16.png", getRenderer());
	backgroundReader->SetRects();

	// need to create Player before validating scene
	if (!ValidateCurrentScene()) {
		OnDestroy();
		return false;
	}

	Collision::debugImage = IMG_Load("DebugCollisionBox.png");
	Collision::debugTexture = SDL_CreateTextureFromSurface(getRenderer(), Collision::debugImage);
	return true;

}


/// Here's the whole game loop
void GameManager::Run() {

	timer->Start();

	while (isRunning) {

		handleEvents();
		timer->UpdateFrameTicks();
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();
		currentScene->PostRenderUpdate(timer->GetDeltaTime());
		/// Keep the event loop running at a proper rate
		SDL_Delay(timer->GetSleepTime(60)); ///60 frames per sec
	}
}

void GameManager::handleEvents()
{
	SDL_Event event;

	// Let's add mouse movement and position
	// https://wiki.libsdl.org/SDL_GetMouseState

	SDL_PumpEvents();  // make sure we have the latest mouse state.

	//https://www.youtube.com/watch?v=SYrRMr4BaD4&list=PLM7LHX-clszBIGsrh7_3B2Pi74AhMpKhj&index=3

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			isRunning = false;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_ESCAPE:
				isRunning = false;
				break;
			case SDL_SCANCODE_Q:
				isRunning = false;
				break;
			case SDL_SCANCODE_DELETE:
				isRunning = false;
				break;
			case SDL_SCANCODE_1:
				LoadScene(1);
				break;
			case SDL_SCANCODE_APOSTROPHE:
				isDebugging = !isDebugging;
				break;
			default:
				break;
			}
		}
		currentScene->HandleEvents(event);
	}
}

GameManager::~GameManager() {}

void GameManager::OnDestroy() {
	if (windowPtr) delete windowPtr;
	if (timer) delete timer;
	if (currentScene) delete currentScene;
	if (player) delete player;
	for (EnemyBody* enemy : enemies) {
		delete enemy;
	}
	enemies.clear();

	for (Bullet* bullet : bullets) {
		delete bullet;
	}

	enemies.clear();
}

// This might be unfamiliar
float GameManager::getSceneHeight() { return currentScene->getyAxis(); }

// This might be unfamiliar
float GameManager::getSceneWidth() { return currentScene->getxAxis(); }

// This might be unfamiliar
Matrix4 GameManager::getProjectionMatrix()
{
	return currentScene->getProjectionMatrix();
}

// This might be unfamiliar
SDL_Renderer* GameManager::getRenderer()
{
	// [TODO] might be missing some SDL error checking
	SDL_Window* window = currentScene->getWindow();
	SDL_Renderer* renderer = SDL_GetRenderer(window);
	return renderer;
}

// This might be unfamiliar
void GameManager::RenderPlayer()
{

	player->Render();
}

void GameManager::LoadScene(int i)
{

	player->Render();
}

void GameManager::RenderEnemy()
{
	for (EnemyBody* enemy : enemies) {
		enemy->Render();
	}
}

void GameManager::RenderBullets()
{
	for (Bullet* bullet : bullets) {
		bullet->Render();
	}
	for (Bullet* bullet : enemyBullets) {
		bullet->Render();
	}

}

void GameManager::RenderTiles()
{
	//for (Tile* tile : tiles) {
	//	tile->Render();
	//}

	grid->RenderGrid();
}

void GameManager::RenderDebug()
{
	if (isDebugging)
	{
		Collision::DisplayDebugCollision(*player, *this);
		for (auto& enemy : enemies)
		{
			Collision::DisplayDebugCollision(*enemy, *this);
		}

		for (Bullet* bullet : bullets)
		{
			if (isDebugging)
			{
				Collision::DisplayDebugCollision(*bullet, *this);
			}
		}

		for (int i = 0; i < grid->GetCollisionTiles()->size(); i++)
		{
			Collision::DisplayDebugCollision(getGrid()->GetCollisionTiles()->at(i), *this);
		}
	}
}

void GameManager::RenderDebugGrid()
{
	if (isDebugging)
	{
		grid->RenderDebugGrid();
	}
}


bool GameManager::ValidateCurrentScene()
{
	if (currentScene == nullptr) {
		return false;
	}
	if (currentScene->OnCreate() == false) {
		return false;
	}
	return true;
}

