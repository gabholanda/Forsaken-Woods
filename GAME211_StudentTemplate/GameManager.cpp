#include "GameManager.h"
#include "Scene1.h"
#include "EnemyBody.h"
#include "Bullet.h"
#include "Collision.h"
#include "Grid.h"
#include <random>
#include "UIText.h"
#include "DecorationTile.h"

GameManager::GameManager() {
	windowPtr = nullptr;
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
	player = nullptr;
	isDebugging = false;
	backgroundReader = nullptr;
	treeReader = nullptr;
	healthUI = nullptr;
	weaponUI = nullptr;
}

bool GameManager::OnCreate()
{
	if (TTF_Init() < 0)
	{
		cout << "Error SDL_ttf:" << TTF_GetError() << endl;
		return false;
	}
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
	grid = new Grid(160, 160, 20, 20, this);
	CreatePlayer();

	// need to create Player before validating scene
	if (!ValidateCurrentScene()) {
		OnDestroy();
		return false;
	}

	// the image is actually 1120x640, 7 rows of 160 and 7 columns of 4
	backgroundReader = new SpritesheetReader(160, 160, 7, 4);
	backgroundReader->LoadFromFile("Ground Tiles_16x16.png", getRenderer());
	backgroundReader->SetRects();

	treeReader = new SpritesheetReader(320, 380, 4, 1);
	treeReader->LoadFromFile("Grouped Tree Tiles_32x38.png", getRenderer());
	treeReader->SetRects();


	CreateBuffs();
	CreateTiles();
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

	const char* fontName = "MainFont.ttf";
	SDL_Color color = { 255,255,255 };
	healthUI = new UIText(getPlayer()->Text(), 24, fontName, getRenderer(), Vec2(25, 550), color);
	weaponUI = new UIText(getPlayer()->GetGun()->Text(), 24, fontName, getRenderer(), Vec2(25, 25), color);
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
		TTF_Quit();
		for (EnemyBody* enemy : enemies)
		{
			delete enemy;
		}
		enemies.clear();

		for (Bullet* bullet : bullets)
		{
			delete bullet;
		}
		bullets.clear();
	}
}

void GameManager::OnRestart()
{
	if (currentScene) delete currentScene;
	for (Bullet* bullet : bullets) {
		delete bullet;
	}
	bullets.clear();
	grid->Clear();

	currentScene = new Scene1(windowPtr->GetSDL_Window(), this);
	CreatePlayer();
	CreateBuffs();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 9);
	CreateTiles();
	CreateEnemies(distrib(gen));

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
	float movementSpeed = 20.0f;

	Gun* gun = Randomizer::getRandomWeapon();

	float scale = 0.5;
	Vec3 size(3.f, 3.f, 0.0f);
	Vec3 position(10.0f, 10.0f, 0.0f);
	//Vec3 position(0.0f, 0.0f, 0.0f);
	Vec3 velocity(0.0f, 0.0f, 0.0f);
	Vec3 acceleration(0.0f, 0.0f, 0.0f);
	float playerHp = 150;

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

void GameManager::CreateTiles()
{
	Tile* exampleTile = new Tile(Vec3(10, 10, 0), 0.0f, 1.f,
		getBackgroundSpritesheetReader()->GetRows(),
		getBackgroundSpritesheetReader()->GetColumns(),
		// This defines which section of the spritesheet we gonna get
		getBackgroundSpritesheetReader()->GetRects()[0][0],
		this);
	exampleTile->setImage(getBackgroundSpritesheetReader()->GetImage());
	exampleTile->setTexture(getBackgroundSpritesheetReader()->GetTexture());


	CollisionTile* exampleCollisionTile = new CollisionTile(Vec3(10, 10, 0), 0.0f, 1.f,
		getBackgroundSpritesheetReader()->GetRows(),
		getBackgroundSpritesheetReader()->GetColumns(),
		Vec3(4.0f, 4.0f, 0.0f),
		// This defines which section of the spritesheet we gonna get
		getBackgroundSpritesheetReader()->GetRects()[0][0],
		this);
	exampleCollisionTile->Tile::setImage(getBackgroundSpritesheetReader()->GetImage());
	exampleCollisionTile->Tile::setTexture(getBackgroundSpritesheetReader()->GetTexture());
	//game->getGrid()->PushTile(exampleCollisionTile, 0);

	// Setting grid borders
	/*TODO: Change this to dynamicly fill the tiles*/
	DecorationTile* treeTile = new DecorationTile(Vec3(10, 10, 0), 0.0f, 0.5f,
		treeReader->GetRows(),
		treeReader->GetColumns(),
		// This defines which section of the spritesheet we gonna get
		treeReader->GetRects()[0][0],
		this);
	treeTile->setImage(treeReader->GetImage());
	treeTile->setTexture(treeReader->GetTexture());
	// Test tile
	// Down
	for (size_t i = 0; i < 60; i++)
	{
		getGrid()->PushTile(treeTile, i);
		getGrid()->PushTile(exampleCollisionTile, i);
	}

	// Left
	for (size_t i = 60; i < 321; i += 20)
	{
		for (size_t j = i; j < i + 3; j++)
		{
			getGrid()->PushTile(treeTile, j);
			getGrid()->PushTile(exampleCollisionTile, j);
		}
	}

	// Up
	for (size_t i = 379; i < 399; i++)
	{
		for (size_t j = i; j > i - 60; j -= 20)
		{
			getGrid()->PushTile(treeTile, j);
			getGrid()->PushTile(exampleCollisionTile, j);
		}
	}

	// Right
	for (size_t i = 76; i < 339; i += 20)
	{
		for (size_t j = i - 3; j < i; j++)
		{
			getGrid()->PushTile(treeTile, j);
			getGrid()->PushTile(exampleCollisionTile, j);
		}
	}

	// Set up arena
	for (size_t i = 63; i < 339; i++)
	{
		getGrid()->PushTile(exampleTile, i);
		if (i % 20 == 12)
		{
			i += 10;
		}
	}
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
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribution(0, getGrid()->GetTiles()->size() - 1);
		int index = distribution(gen);
		Vec3 positionEnemy = getGrid()->GetTiles()->at(index).getPos();
		Vec3 velocityEnemy(0.0f, 0.0f, 0.0f);
		Vec3 accelerationEnemy(0.0f, 0.0f, 0.0f);
		Gun* randomEnemyGun = Randomizer::getRandomWeapon();
		// Giving some random HP
		distribution = std::uniform_int_distribution<>(10, 35);
		float enemyHp = distribution(gen);

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

void GameManager::RenderUI()
{
	healthUI->setText(getPlayer()->Text());
	weaponUI->setText(getPlayer()->GetGun()->Text());
	healthUI->Render();
	weaponUI->Render();
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

