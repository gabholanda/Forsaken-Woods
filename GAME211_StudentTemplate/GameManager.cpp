#include "GameManager.h"
#include "Scene1.h"
#include "EnemyBody.h"
#include "Bullet.h"
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

bool GameManager::OnCreate()
{

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

	currentScene = new Scene1(windowPtr->GetSDL_Window(), this);

	/* Grid needs to be same dimension as our sprites */
	grid = new Grid(160, 160, 15, 15, this);
	CreatePlayer();
	CreateBuffs();
	CreateBuffBody(1);
	CreateEnemies(1);

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
	for (auto& buffBody : buffBodies) {
		if (buffBody->OnCreate() == false) {
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
		if (isRestarting)
		{
			OnRestart();
		}
		SDL_Delay(timer->GetSleepTime(60)); ///60 frames per sec
	}
	OnDestroy();
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

void GameManager::OnDestroy()
{
	if (!isRunning)
	{
		if (windowPtr) delete windowPtr;
		if (timer) delete timer;
		if (player) delete player;
		for (EnemyBody* enemy : enemies)
		{
			delete enemy;
		}
		enemies.clear();

		for (Bullet* bullet : bullets)
		{
			delete bullet;
		}
	}
	bullets.clear();
}

void GameManager::OnRestart()
{
	if (currentScene) delete currentScene;
	for (Bullet* bullet : bullets) {
		delete bullet;
	}

	bullets.clear();

	currentScene = new Scene1(windowPtr->GetSDL_Window(), this);

	CreatePlayer();
	CreateBuffs();
	CreateEnemies(1);

	if (player->OnCreate() == false) {
		OnDestroy();
		isRunning = false;
	}

	for (auto& currentEnemy : enemies) {
		if (currentEnemy->OnCreate() == false) {
			OnDestroy();
			isRunning = false;
		}
	}

	if (!ValidateCurrentScene()) {
		OnDestroy();
		isRunning = false;
	}
	isRestarting = false;
}

void GameManager::SetRestart(bool isRestarting_)
{
	isRestarting = isRestarting_;
}

void GameManager::CreatePlayer()
{
	if (player) delete player;
	// create player
	float mass = 1.0f;
	float orientation = 0.0f;
	float rotation = 0.0f;
	float angular = 0.0f;
	float movementSpeed = 5.0f;

	Gun* gun = Randomizer::getRandomWeapon();

	float scale = 0.5;
	Vec3 size(3.f, 3.f, 0.0f);
	Vec3 position(0.5f * currentScene->getxAxis(), 0.5f * currentScene->getyAxis(), 0.0f);
	//Vec3 position(0.0f, 0.0f, 0.0f);
	Vec3 velocity(0.0f, 0.0f, 0.0f);
	Vec3 acceleration(0.0f, 0.0f, 0.0f);
	float playerHp = 100;

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
		this,
		playerHp
	);

	gun->SetGunOwner(player);
}

void GameManager::CreateBuffs()
{
	if (buffManager) delete buffManager;

	float mass = 0.0f;
	float orientation = 0.0f;
	float rotation = 0.0f;
	float angular = 0.0f;
	float movementSpeed = 0.0f;
	float scale = 0.5;
	Vec3 size(3.f, 3.f, 0.0f);
	Vec3 position(0.5f * currentScene->getxAxis(), 0.5f * currentScene->getyAxis(), 0.0f);
	//Vec3 position(0.0f, 0.0f, 0.0f);
	Vec3 velocity(0.0f, 0.0f, 0.0f);
	Vec3 acceleration(0.0f, 0.0f, 0.0f);
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
}

void GameManager::CreateBuffBody(int quantity)
{
	if (buffBodies.size() > 0)
	{
		for (Buff* buffBody : buffBodies) {
			delete buffBody;
		}
		buffBodies.clear();
	}

	for (int i = 0; i < quantity; i++) {
		float massBuff = 1.0f;
		float orientationBuff = 0.0f;
		float rotationBuff = 0.0f;
		float angularBuff = 0.0f;
		float movementSpeedBuffy = 1.0f;
		float scaleBuff = 0.5;
		Vec3 sizeBuff(3.f, 3.f, 0.0f);
		Vec3 positionBuff(0.3f * currentScene->getxAxis(), 0.3f * currentScene->getyAxis(), 0.0f);
		Vec3 velocityBuff(0.0f, 0.0f, 0.0f);
		Vec3 accelerationBuff(0.0f, 0.0f, 0.0f);
		float movementSpeedBuff = 0.0f;
		Gun* randomEnemyBuff = Randomizer::getRandomWeapon();

		Buff* newBuff = new Buff(
			positionBuff,
			velocityBuff,
			accelerationBuff,
			sizeBuff,
			massBuff,
			orientationBuff,
			rotationBuff,
			angularBuff,
			movementSpeedBuff,
			scaleBuff,
			this
		);
		buffBodies.push_back(newBuff);
	}
}

void GameManager::CreateEnemies(int quantity)
{
	if (enemies.size() > 0)
	{
		for (EnemyBody* enemy : enemies) {
			delete enemy;
		}
		enemies.clear();
	}

	for (int i = 0; i < quantity; i++) {
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
		Gun* randomEnemyGun = Randomizer::getRandomWeapon();
		float enemyHp = 100;

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
			this,
			enemyHp
		);
		randomEnemyGun->SetEnemyGunOwner(newEnemy);
		enemies.push_back(newEnemy);
	}
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

void GameManager::RenderBuffBody()
{
	for (Buff* buffBody : buffBodies) {
		buffBody->Render();
	}
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
			Collision::DisplayDebugCollision(*bullet, *this);
		}

		for (Bullet* bullet : enemyBullets)
		{
			Collision::DisplayDebugCollision(*bullet, *this);
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

