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
	stageUI = nullptr;
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


	// need to create Player before validating scene
	if (!ValidateCurrentScene()) {
		OnDestroy();
		return false;
	}

	// the image is actually 1120x640, 7 rows of 160 and 7 columns of 4
	backgroundReader = new SpritesheetReader(160, 160, 7, 4);
	backgroundReader->LoadFromFile("Ground Tiles_16x16.png", getRenderer());
	backgroundReader->SetRects();

	treeReader = new SpritesheetReader(480, 380, 2, 1);
	treeReader->LoadFromFile("GroupedTreeTiles_48x38.png", getRenderer());
	treeReader->SetRects();


	flowerReader = new SpritesheetReader(160, 160, 9, 1);
	flowerReader->LoadFromFile("DecorativeTiles1_16x16.png", getRenderer());
	flowerReader->SetRects();

	insideTreeReader = new SpritesheetReader(320, 380, 2, 2);
	insideTreeReader->LoadFromFile("Seperate Tree Tiles_32x38.png", getRenderer());
	insideTreeReader->SetRects();

	CreateBuffs();
	CreateTiles();
	CreatePlayer();
	CreateEnemies(1);
	CreateBuffBody(1);

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

	const char* fontName = "MainFont.ttf";
	SDL_Color color = { 255,255,255 };
	healthUI = new UIText(getPlayer()->Text(), 24, fontName, getRenderer(), Vec2(25, 550), color);
	weaponUI = new UIText(getPlayer()->GetGun()->Text(), 24, fontName, getRenderer(), Vec2(25, 25), color);
	stageUI = new UIText("1", 24, fontName, getRenderer(), Vec2(925, 25), color);
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
	CreateTiles();
	CreatePlayer();
	CreateBuffs();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 9);
	CreateEnemies(distrib(gen));
	std::random_device rd2;
	std::mt19937 gen2(rd2());
	std::uniform_int_distribution<> distrib2(0, 3);
	CreateBuffBody(distrib2(gen2));

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
	for (auto& buffBody : buffBodies) {
		if (buffBody->OnCreate() == false) {
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

void GameManager::OnWin()
{
	if (stageNumber < 101) {
		stageNumber += 1;
		std::cout << "stageNumber:" << stageNumber << std::endl;
		if (currentScene) delete currentScene;
		for (Bullet* bullet : bullets) {
			delete bullet;
		}
		bullets.clear();
		grid->Clear();

		currentScene = new Scene1(windowPtr->GetSDL_Window(), this);
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(0, 9);
		CreateTiles();
		PlayerNextLevel();
		CreateEnemies(distrib(gen));
		std::random_device rd2;
		std::mt19937 gen2(rd2());
		std::uniform_int_distribution<> distrib2(0, 3);
		CreateBuffBody(distrib2(gen2));

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
		for (auto& buffBody : buffBodies) {
			if (buffBody->OnCreate() == false) {
				OnDestroy();
				isRunning = false;
			}
		}

		if (!ValidateCurrentScene()) {
			OnDestroy();
			isRunning = false;
		}
	}
}

void GameManager::SetRestart(bool isRestarting_)
{
	isRestarting = isRestarting_;
}

void GameManager::CreatePlayer()
{
	stageNumber = 1;
	if (player) delete player;
	// create player
	float mass = 1.0f;
	float orientation = 0.0f;
	float rotation = 0.0f;
	float angular = 0.0f;
	float movementSpeed = 15.0f;

	Gun* gun = Randomizer::getRandomWeapon();

	float scale = 0.5;
	Vec3 size(3.f, 3.f, 0.0f);
	//Vec3 position (15.0f, 15.0f, 0.0f);
	std::random_device rd2;
	std::mt19937 gen2(rd2());
	std::uniform_int_distribution<> distribution2(3 * getGrid()->GetTiles()->size() / 25 + 5, 22 * getGrid()->GetTiles()->size() / 25 - 5);
	int playerSpawnIndex = distribution2(gen2);
	Vec3 position = getGrid()->GetTiles()->at(playerSpawnIndex).getPos();
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
	player->setPlayerPositionIndex(playerSpawnIndex);
	gun->SetGunOwner(player);
	gun->SaveInitialStats();

}

void GameManager::PlayerNextLevel()
{
	float currentMaxHp = player->getMaxHp();
	float currentHp = player->getHp();
	float newHp = currentHp + static_cast<float>(static_cast<int>(currentMaxHp / 3.0f));
	player->setHp(std::min(newHp, currentMaxHp));
	Gun* gun = Randomizer::getRandomWeapon();
	player->setPos(Randomizer::getRandomGridPosition(grid));

	player->GetGun()->SaveState();
	gun->SaveAdditionalStats();


	player->SetGun(gun);

	gun->SaveInitialStats();
	gun->ApplyAdditionalStats();


	gun->SetGunOwner(player);
}

void GameManager::CreateTiles()
{
	Tile* exampleTile = new Tile(Vec3(10, 10, 0), 0.0f, 1.0f,
		getBackgroundSpritesheetReader()->GetRows(),
		getBackgroundSpritesheetReader()->GetColumns(),
		// This defines which section of the spritesheet we gonna get
		getBackgroundSpritesheetReader()->GetRects()[0][0],
		this);
	exampleTile->setImage(getBackgroundSpritesheetReader()->GetImage());
	exampleTile->setTexture(getBackgroundSpritesheetReader()->GetTexture());


	CollisionTile* leftTreeTile = new CollisionTile(Vec3(10, 10, 0), 0.0f, 0.5f,
		treeReader->GetRows(),
		treeReader->GetColumns(),
		Vec3(4.0f, 4.0f, 0.0f),
		// This defines which section of the spritesheet we gonna get
		treeReader->GetRects()[0][0],
		this);
	leftTreeTile->Tile::setImage(treeReader->GetImage());
	leftTreeTile->Tile::setTexture(treeReader->GetTexture());

	DecorationTile* backgroundDecorationTile = new DecorationTile(Vec3(10, 10, 0), 0.0f, 1.0f,
		backgroundReader->GetRows(),
		backgroundReader->GetColumns(),
		backgroundReader->GetRects()[0][0],
		this);
	backgroundDecorationTile->Tile::setImage(backgroundReader->GetImage());
	backgroundDecorationTile->Tile::setTexture(backgroundReader->GetTexture());

	CollisionTile* rightTreeTile = new CollisionTile(Vec3(10, 10, 0), 0.0f, 0.5f,
		treeReader->GetRows(),
		treeReader->GetColumns(),
		Vec3(4.0f, 4.0f, 0.0f),
		treeReader->GetRects()[0][1],
		this);
	rightTreeTile->Tile::setImage(treeReader->GetImage());
	rightTreeTile->Tile::setTexture(treeReader->GetTexture());

	CollisionTile* treeCollisionTile = new CollisionTile(Vec3(10, 10, 0), 0.0f, 0.5f,
		insideTreeReader->GetRows(),
		insideTreeReader->GetColumns(),
		Vec3(2.0f, 4.0f, 0.0f),
		// This defines which section of the spritesheet we gonna get
		insideTreeReader->GetRects()[0][0],
		this);
	treeCollisionTile->Tile::setImage(insideTreeReader->GetImage());
	treeCollisionTile->Tile::setTexture(insideTreeReader->GetTexture());

	// Up
	for (size_t i = 379; i < 400; i++)
	{
		for (size_t j = i; j > i - 60; j -= 20)
		{
			getGrid()->PushTile(rightTreeTile, j);
			getGrid()->PushTile(backgroundDecorationTile, j);
		}
	}

	// Down
	for (size_t i = 0; i < 60; i++)
	{
		getGrid()->PushTile(leftTreeTile, i);
		getGrid()->PushTile(backgroundDecorationTile, i);
	}

	// Left
	for (size_t i = 60; i < 321; i += 20)
	{
		for (size_t j = i; j < i + 3; j++)
		{
			getGrid()->PushTile(leftTreeTile, j);
			getGrid()->PushTile(backgroundDecorationTile, j);
		}
	}

	// Right
	for (size_t i = 80; i < 342; i += 20)
	{
		for (size_t j = i - 3; j < i; j++)
		{
			getGrid()->PushTile(rightTreeTile, j);
			getGrid()->PushTile(backgroundDecorationTile, j);
		}
	}

	// Set up arena



	for (size_t i = 63; i < 339; i++)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribution(1, 10);
		int index = distribution(gen);
		if (index == 2)
		{
			distribution = std::uniform_int_distribution<>(0, flowerReader->GetRects()[0].size() - 1);
			int flowerIndex = distribution(gen);
			DecorationTile* flowerTile = new DecorationTile(Vec3(10, 10, 0), 0.0f, 0.5f,
				flowerReader->GetRows(),
				flowerReader->GetColumns(),
				flowerReader->GetRects()[0][flowerIndex],
				this);
			flowerTile->setImage(flowerReader->GetImage());
			flowerTile->setTexture(flowerReader->GetTexture());
			getGrid()->PushTile(flowerTile, i);
		}
		else
		{
			distribution = std::uniform_int_distribution<>(1, 30);
			int treeIndex = distribution(gen);
			if (treeIndex == 2)
			{
				treeCollisionTile->Tile::setImage(insideTreeReader->GetImage());
				treeCollisionTile->Tile::setTexture(insideTreeReader->GetTexture());
				getGrid()->PushTile(treeCollisionTile, i);
				getGrid()->PushTile(backgroundDecorationTile, i);
				continue;
			}
		}
		getGrid()->PushTile(exampleTile, i);
		if (i % 20 == 16)
		{
			i += 6;
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
		Vec3 sizeBuff(1.5f, 1.5f, 0.0f);
		std::random_device rd2;
		std::mt19937 gen2(rd2());
		std::uniform_int_distribution<> distribution2(0, getGrid()->GetTiles()->size() - 1);
		int index2 = distribution2(gen2);
		Vec3 positionBuff = getGrid()->GetTiles()->at(index2).getPos();
		Vec3 velocityBuff(0.0f, 0.0f, 0.0f);
		Vec3 accelerationBuff(0.0f, 0.0f, 0.0f);
		float movementSpeedBuff = 0.0f;

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

	Vec3 playerPosition = getPlayer()->getPos();
	float playerSpawnIndex = getPlayer()->GetPlayerSpawnIndex();
	std::vector<Tile*> validTiles = getGrid()->GetValidTiles(playerPosition, playerSpawnIndex);


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

		if (validTiles.empty()) {
			std::cout << "no tile";
			break;
		}

		std::uniform_int_distribution<> distribution(0, validTiles.size() - 1);
		int index = distribution(gen);
		Vec3 positionEnemy = validTiles[index]->getPos();
		std::cout << "enemy spawned at " << positionEnemy;
		//Vec3 positionEnemy = Randomizer::getRandomGridPosition(grid);
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
	std::string s = std::to_string(stageNumber);
	char* result = new char[s.length() + 1];

	// Copy the contents of nameString to the newly allocated memory using strcpy_s
	strcpy_s(result, s.length() + 1, s.c_str());
	stageUI->setText(result);

	healthUI->Render();
	weaponUI->Render();
	stageUI->Render();
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

