#include "GameManager.h"
#include "Scene1.h"
#include "EnemyBody.h"
#include "Collision.h"
#include "Grid.h"
#include <random>
#include "UIText.h"
#include "DecorationTile.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "gl3w.h"
#include <thread>
#include <future>
#include <mutex>
#include <vector>
#include <fstream>


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
	buffUI = nullptr;
	InstructionsUI1 = nullptr;
	InstructionsUI2 = nullptr;
	InstructionsUI3 = nullptr;
	InstructionsUI4 = nullptr;
}

bool GameManager::OnCreate()
{
	if (TTF_Init() < 0)
	{
		cout << "Error SDL_ttf:" << TTF_GetError() << endl;
		return false;
	}
	// https://lazyfoo.net/tutorials/SDL/21_sound_effects_and_music/index.php
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	soundEngine = irrklang::createIrrKlangDevice();
	if (!soundEngine) {
		std::cerr << "Failed to initialize IrrKlang!" << std::endl;
		return -1;
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
	buff = new DamageBuff;

	// need to create Player before validating scene
	if (!ValidateCurrentScene()) {
		OnDestroy();
		return false;
	}
	/* Grid needs to be same dimension as our sprites */
	grid = new Grid(160, 160, 20, 20, this, currentScene->getCamera());



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
	bulletPool = new MemoryPool<Bullet>(1000);     // Pre-allocate bullets for player
	enemyPool = new MemoryPool<EnemyBody>(100);    // Pre-allocate enemies

	InitializeController();
	CreateBuffs();
	CreateTiles();
	CreatePlayer();
	CreateEnemies(1);
	CreateBuffBody(1);

	backgroundMusic = soundEngine->play2D("Retro Forest.mp3", true, true, true);
	if (!backgroundMusic)
	{
		printf("Failed to load background music: Retro Forest.mp3\n");
		return false;
	}
	backgroundMusic->setVolume(0.8f);
	backgroundMusic->setIsPaused(false); // Start playing
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
	buffUI = new UIText(getBuff()->Text(), 24, fontName, getRenderer(), Vec2(550, 25), color);
	InstructionsUI1 = new UIText("test", 24, fontName, getRenderer(), Vec2(25, 100), color);
	InstructionsUI2 = new UIText("test", 24, fontName, getRenderer(), Vec2(25, 200), color);
	InstructionsUI3 = new UIText("test", 24, fontName, getRenderer(), Vec2(25, 300), color);
	InstructionsUI4 = new UIText("test", 24, fontName, getRenderer(), Vec2(25, 400), color);
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
			case SDL_SCANCODE_F5: // Save game
				SaveGame("savegame.bin");
				break;
			case SDL_SCANCODE_F9: // Load game
				LoadGame("savegame.bin");
				break;
			case SDL_SCANCODE_SPACE:
				OnRestart();
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
		delete bulletPool;
		delete enemyPool;


		if (soundEngine)
		{
			soundEngine->drop(); // Release the engine
			soundEngine = nullptr;
		}
		backgroundMusic = NULL;
		Mix_Quit();
		IMG_Quit();
	}
}

void GameManager::OnRestart()
{
	if (currentScene) delete currentScene;
	for (Buff* buff : buffBodies)
	{
		delete buff;
	}

	// Clear lists
	buffBodies.clear();
	enemies.clear();
	bullets.clear();
	grid->Clear();

	currentScene = new Scene1(windowPtr->GetSDL_Window(), this);
	CreateTiles();
	CreateBuffs();
	CreatePlayer();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 9);

	CreateEnemies(distrib(gen));
	std::random_device rd2;
	std::mt19937 gen2(rd2());
	std::uniform_int_distribution<> distrib2(1, 4);
	CreateBuffBody(distrib2(gen2));

	if (player->OnCreate() == false) {
		OnDestroy();
		isRunning = false;
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


		for (Buff* buff : buffBodies)
		{
			delete buff;
		}

		// Clear lists
		buffBodies.clear();
		enemies.clear();
		bullets.clear();
		grid->Clear();

		// Start creating stuff
		currentScene = new Scene1(windowPtr->GetSDL_Window(), this);
		CreateTiles();
		PlayerNextLevel();
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(1, 9);
		CreateEnemies(distrib(gen));


		std::random_device rd2;
		std::mt19937 gen2(rd2());
		std::uniform_int_distribution<> distrib2(1, 4);
		CreateBuffBody(distrib2(gen2));

		if (player->OnCreate() == false) {
			OnDestroy();
			isRunning = false;
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
	float newHp = currentHp + static_cast<float>(static_cast<int>(currentMaxHp / 2.0f));
	player->setHp(min(newHp, currentMaxHp));
	Gun* gun = Randomizer::getRandomWeapon();
	player->setPos(Randomizer::getRandomGridPosition(grid));

	player->GetGun()->SaveState();
	player->GetGun()->SaveAdditionalStats();

	Gun* previousGun;
	previousGun = player->GetGun();

	player->SetGun(gun);

	gun->SaveInitialStats();
	gun->ApplyAdditionalStats(previousGun);


	gun->SetGunOwner(player);
}

void GameManager::CreateTiles()
{
	grid->Clear();

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
				if (i % 20 == 16)
				{
					i += 6;
				}
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

void GameManager::CreateBuffBody(int quantity) {
	// Clear existing buffs
	{
		std::lock_guard<std::mutex> lock(buffMutex);
		for (Buff* buffBody : buffBodies) {
			delete buffBody;
		}
		buffBodies.clear();
	}

	// Prepare a vector of futures to hold results from async calls
	std::vector<std::future<Buff*>> futures;
	futures.reserve(quantity);

	for (int i = 0; i < quantity; i++) {
		//USING ASYNC to ensure it runs asynchronously  
		futures.push_back(std::async(std::launch::async, [this]() -> Buff* {
			// Each thread gets its own random generator
			std::random_device rd2;
			std::mt19937 gen2(rd2());
			std::uniform_int_distribution<> distribution2(0, getGrid()->GetTiles()->size() - 1);
			int index2 = distribution2(gen2);
			Vec3 positionBuff = getGrid()->GetTiles()->at(index2).getPos();

			float massBuff = 1.0f;
			float orientationBuff = 0.0f;
			float rotationBuff = 0.0f;
			float angularBuff = 0.0f;
			float movementSpeedBuff = 0.0f;
			float scaleBuff = 0.5f;
			Vec3 sizeBuff(1.5f, 1.5f, 0.0f);
			Vec3 velocityBuff(0.0f, 0.0f, 0.0f);
			Vec3 accelerationBuff(0.0f, 0.0f, 0.0f);

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
			return newBuff;
			}));
	}

	// Gather results and push them into the main buffBodies vector
	for (auto& f : futures) {
		Buff* createdBuff = f.get();
		std::lock_guard<std::mutex> lock(buffMutex);
		buffBodies.push_back(createdBuff);
	}
}

void GameManager::CreateEnemies(int quantity)
{
	// If we currently have enemies, destroy them properly (returning them to the pool)
	while (!enemies.empty()) {
		DestroyEnemy(enemies.back());
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
		float scaleEnemy = 0.5f;
		Vec3 sizeEnemy(3.f, 3.f, 0.0f);

		if (validTiles.empty()) {
			std::cout << "No valid tile found for enemy spawn.\n";
			break;
		}

		// Use a random tile from the validTiles list
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> tileDist(0, static_cast<int>(validTiles.size()) - 1);
		int index = tileDist(gen);
		Vec3 positionEnemy = validTiles[index]->getPos();
		std::cout << "Enemy spawned at: " << positionEnemy << std::endl;

		Vec3 velocityEnemy(0.0f, 0.0f, 0.0f);
		Vec3 accelerationEnemy(0.0f, 0.0f, 0.0f);

		// Get a random gun for the enemy
		Gun* randomEnemyGun = Randomizer::getRandomWeapon();

		// Give the enemy some random HP between 10 and 35
		std::uniform_int_distribution<> hpDist(10, 35);
		float enemyHp = static_cast<float>(hpDist(gen));

		// Now create the enemy using the memory pool
		EnemyBody* newEnemy = CreateEnemy(
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
			enemyHp
		);

		// Assign the gun owner to this newly created enemy
		if (randomEnemyGun) {
			randomEnemyGun->SetEnemyGunOwner(newEnemy);
		}
	}
	std::cout << "ENEMIES SPAWNED " << enemies.size() << std::endl;
}




EnemyBody* GameManager::CreateEnemy(
	Gun* gun_,
	const Vec3& pos_, const Vec3& vel_, const Vec3& accel_,
	const Vec3& size_,
	float mass_,
	float orientation_,
	float rotation_,
	float angular_,
	float movementSpeed_,
	float scale_,
	float enemyHp_
) {
	
	EnemyBody* e = enemyPool->getObject();
	e->SetParameters(gun_, pos_, vel_, accel_, size_, mass_, orientation_, rotation_, angular_, movementSpeed_, scale_, this, enemyHp_);
	e->OnCreate();
	e->setMarkedForDeletion(false);
	enemies.push_back(e);
	return e;
}

void GameManager::DestroyEnemy(EnemyBody* e) {

	enemyPool->ReturnObject(e);
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

void GameManager::InitializeController()
{
	// Initialize SDL GameController subsystem
	if (SDL_Init(SDL_INIT_GAMECONTROLLER) != 0) {
		printf("SDL_Init Error: %s\n", SDL_GetError());
		return; // Exit the function if SDL_Init fails
	}

	SDL_GameController* controller = nullptr;

	// Check if a controller is connected
	if (SDL_NumJoysticks() > 0) {
		controller = SDL_GameControllerOpen(0);
		if (controller == nullptr) {
			printf("Could not open gamecontroller: %s\n", SDL_GetError());
		}
	}

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
	if (!player->getIsBeginningOfGame()) {
		healthUI->setText(getPlayer()->Text());
		weaponUI->setText(getPlayer()->GetGun()->Text());

		std::string s = std::to_string(stageNumber);
		char* result = new char[s.length() + 1];

		// Copy the contents of nameString to the newly allocated memory using strcpy_s
		strcpy_s(result, s.length() + 1, s.c_str());
		stageUI->setText(result);
		if (getBuff()->getCanCollect() == true)
		{
			buffUI->setText(getBuff()->Text());
			buffUI->Render();
		}
		healthUI->Render();
		weaponUI->Render();
		stageUI->Render();

	}
	else if (player->getIsBeginningOfGame()) {
		InstructionsUI1->setText("You have been trapped in the ForsakenWoods...");
		InstructionsUI2->setText("WASD To Move, R To reload, Right Click to Dash");
		InstructionsUI3->setText("Walk into shiny rocks to get stronger! How far can you get?");
		InstructionsUI4->setText("Press Any Key To Continue");
		InstructionsUI1->Render();
		InstructionsUI2->Render();
		InstructionsUI3->Render();
		InstructionsUI4->Render();

	}
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

void GameManager::StartRenderImGui() {
	// Start a new ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug Window");

	if (player) {
		ImGui::Text("Player Stats");
		ImGui::Separator();
		ImGui::Text("Max HP: %.2f", player->getMaxHp());
		ImGui::Text("Current Speed: %.2f", player->getMovementSpeed());
		ImGui::Text("Dash Cooldown: %.2f", player->getDashCooldown());
		ImGui::Text("Movement Speed: %.2f", player->getMovementSpeed());
		ImGui::Text("Gun: %s", player->GetGun()->GetName());
		ImGui::Text("Reload Speed: %.2f", player->GetGun()->GetReloadSpeed());
		ImGui::Text("Fire Rate: %.2f", player->GetGun()->GetFireRate());
		ImGui::Text("Damage: %.2f", player->GetGun()->GetDamage());
	}
	ImGui::Separator(); 
	ImGui::Text("Apply Buffs");
	ImGui::Separator(); 
	PlayerBody* player = getPlayer();
	if (player) {
		BuffManager* buffManager = getBuffManager();

		// Ensure BuffManager exists
		if (buffManager) {
			// Apply Health Buff
			if (ImGui::Button("Apply Health Buff")) {
				for (Buff* buff : buffManager->GetBuffs()) {
					if (dynamic_cast<HealthBuff*>(buff)) {
						buff->ApplyBuff(player);
						break;
					}
				}
			}

			// Apply Movement Speed Buff
			if (ImGui::Button("Apply Movement Speed Buff")) {
				for (Buff* buff : buffManager->GetBuffs()) {
					if (dynamic_cast<MovementSpeedBuff*>(buff)) {
						buff->ApplyBuff(player);
						break;
					}
				}
			}

			// Apply Fire Rate Buff
			if (ImGui::Button("Apply Fire Rate Buff")) {
				for (Buff* buff : buffManager->GetBuffs()) {
					if (dynamic_cast<FireRateBuff*>(buff)) {
						buff->ApplyBuff(player);
						break;
					}
				}
			}

			// Apply Reload Speed Buff
			if (ImGui::Button("Apply Reload Speed Buff")) {
				for (Buff* buff : buffManager->GetBuffs()) {
					if (dynamic_cast<ReloadSpeedBuff*>(buff)) {
						buff->ApplyBuff(player);
						break;
					}
				}
			}

			// Apply Damage Buff
			if (ImGui::Button("Apply Damage Buff")) {
				for (Buff* buff : buffManager->GetBuffs()) {
					if (dynamic_cast<DamageBuff*>(buff)) {
						buff->ApplyBuff(player);
						break;
					}
				}
			}
		}
	}


	ImGui::Separator(); 
	ImGui::Text("Settings");
	ImGui::Separator(); 
	// Create a slider for volume, clamping the values between 0.0f and 2.0f
	if (ImGui::SliderFloat("Volume", &volume, 0.0f, 2.0f)) {

		getSoundEngine()->setSoundVolume(volume);
	}

	ImGui::End();

	// Render the ImGui data
	ImGui::Render();
	glClear(GL_COLOR_BUFFER_BIT); // Clear the OpenGL buffer before rendering ImGui
}

void GameManager::EndRenderImGui()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

Bullet* GameManager::CreateBullet(
	Gun* owningGun_,
	const Vec3& pos_, const Vec3& vel_, const Vec3& accel_,
	const Vec3& size_,
	float mass_,
	float orientation_,
	float rotation_,
	float angular_,
	float movementSpeed_,
	float scale_,
	float lifeTime_
) {
	// Decide which pool to use based on owningGun_ if needed
	// For simplicity, use bulletPool for player bullets, enemyBulletPool for enemy bullets.
	// If owningGun_ belongs to enemy, use enemyBulletPool, else bulletPool.

	MemoryPool<Bullet>* poolToUse = bulletPool;
	// If we have a way to check if it's an enemy gun:
	// if (dynamic_cast<EnemyBody*>(owningGun_->GetGunOwnerEnemy()) != nullptr) poolToUse = enemyBulletPool;

	Bullet* b = poolToUse->getObject();
	b->SetParameters(owningGun_, pos_, vel_, accel_, size_, mass_, orientation_, rotation_, angular_, movementSpeed_, scale_, lifeTime_, this);
	b->OnCreate();
	return b;
}

void GameManager::DestroyBullet(Bullet* b) {
	
			bulletPool->ReturnObject(b);	
	

}

void GameManager::SaveGame(const std::string& filePath) {
	GameState gameState;
	gameState.playerPosition = player->getPos();
	gameState.playerHealth = player->getHp();
	gameState.stageNumber = stageNumber;

	std::ofstream outFile(filePath, std::ios::binary);
	if (!outFile) {
		std::cerr << "Failed to open file for saving.\n";
		return;
	}

	// Serialize player position
	outFile.write(reinterpret_cast<char*>(&gameState.playerPosition), sizeof(gameState.playerPosition));

	// Serialize player health
	outFile.write(reinterpret_cast<char*>(&gameState.playerHealth), sizeof(gameState.playerHealth));

	// Serialize stage number
	outFile.write(reinterpret_cast<char*>(&gameState.stageNumber), sizeof(gameState.stageNumber));

	outFile.close();
	std::cout << "Game saved successfully.\n";
}

void GameManager::LoadGame(const std::string& filePath) {
	GameState gameState;

	std::ifstream inFile(filePath, std::ios::binary);
	if (!inFile) {
		std::cerr << "Failed to open file for loading.\n";
		return;
	}

	// Deserialize player position
	inFile.read(reinterpret_cast<char*>(&gameState.playerPosition), sizeof(gameState.playerPosition));

	// Deserialize player health
	inFile.read(reinterpret_cast<char*>(&gameState.playerHealth), sizeof(gameState.playerHealth));

	// Deserialize stage number
	inFile.read(reinterpret_cast<char*>(&gameState.stageNumber), sizeof(gameState.stageNumber));

	inFile.close();

	// Apply the loaded data to the player and game manager
	player->setPos(gameState.playerPosition);
	player->setHp(gameState.playerHealth);
	stageNumber = gameState.stageNumber;

	std::cout << "Game loaded successfully.\n";
}

