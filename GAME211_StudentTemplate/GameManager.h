#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <SDL.h>
#include <iostream>
#include <vector>
#include "Window.h"
#include "Timer.h"
#include "Scene.h"
#include "PlayerBody.h"
#include "SpritesheetReader.h"
#include "Tile.h"
#include "Bullet.h"
#include "BuffManager.h"
#include <SDL_mixer.h>
#include "MemoryPool.h"
#include <thread>
#include <future>
#include <mutex>
#include <vector>
#include <algorithm>
#include <irrKlang.h>


class EnemyBody;
class Buff;
class Bullet;
class Grid;
class UIText;
class Gun;
class Bullet;

struct GameState {
	Vec3 playerPosition;
	float playerHealth;
	int stageNumber;
};




class GameManager {
private:
	class Window* windowPtr;
	class Timer* timer;
	bool isRunning;
	bool isDebugging;
	bool isRestarting;
	class Scene* currentScene;
	Grid* grid;
	class PlayerBody* player;
	class BuffManager* buffManager;
	class Buff* buff;
	int stageNumber;
	MemoryPool<Bullet>* bulletPool;
	MemoryPool<EnemyBody>* enemyPool;
	std::vector<Bullet*> bullets;
	std::vector<Bullet*> enemyBullets;
	std::vector<Buff*> buffBodies;
	std::vector<Tile*> tiles;
	UIText* healthUI;
	UIText* weaponUI;
	UIText* stageUI;
	UIText* buffUI;
	UIText* InstructionsUI1;
	UIText* InstructionsUI2;
	UIText* InstructionsUI3;
	UIText* InstructionsUI4;
	irrklang::ISoundEngine* soundEngine = nullptr;
	irrklang::ISound* backgroundMusic = nullptr;
	SpritesheetReader* backgroundReader;
	SpritesheetReader* treeReader;
	SpritesheetReader* flowerReader;
	SpritesheetReader* insideTreeReader;
	std::mutex buffMutex;
	std::mutex enemiesMutex;
	std::vector<EnemyBody*> enemies;
public:
	GameManager();
	~GameManager();
	bool OnCreate();
	void OnDestroy();
	void OnRestart();
	void OnWin();
	void CreatePlayer();
	void PlayerNextLevel();
	void CreateTiles();
	void CreateBuffs();
	void CreateBuffBody(int quantity);
	void CreateEnemies(int quantity);
	// These might be unfamiliar
	float getSceneHeight();
	float getSceneWidth();
	Matrix4 getProjectionMatrix();
	void InitializeController();

	Bullet* CreateBullet(Gun* owningGun_, const Vec3& pos_, const Vec3& vel_, const Vec3& accel_, const Vec3& size_, float mass_, float orientation_, float rotation_, float angular_, float movementSpeed_, float scale_, float lifeTime_);
	void DestroyBullet(Bullet* b);
	EnemyBody* CreateEnemy(Gun* gun_, const Vec3& pos_, const Vec3& vel_, const Vec3& accel_, const Vec3& size_, float mass_, float orientation_, float rotation_, float angular_, float movementSpeed_, float scale_, float enemyHp_);
	void DestroyEnemy(EnemyBody* e);

	UIText* getBuffUI() { return buffUI; }

	void SetRestart(bool isRestarting_);
	bool GetRestart() const { return isRestarting; };
	PlayerBody* getPlayer() { return player; }
	Grid* getGrid() { return grid; }
	std::vector<EnemyBody*>* getEnemies() { return &enemies; }
	std::vector<Bullet*>* getBullets() { return &bullets; }
	std::vector<Bullet*>* getEnemyBullets() { return &enemyBullets; }
	std::vector<Buff*>* getBuffBodies() { return &buffBodies; }
	std::vector<Tile*>* getTiles() { return &tiles; }
	SpritesheetReader* getBackgroundSpritesheetReader() { return backgroundReader; }
	SpritesheetReader* getTreeSpritesheetReader() { return treeReader; }
	BuffManager* getBuffManager() { return buffManager; }
	irrklang::ISoundEngine* getSoundEngine() { return soundEngine; };
	Buff* getBuff() { return buff; }
	void setBuff(Buff* buff_) { buff = buff_; }
	SDL_Renderer* getRenderer();
	void RenderUI();
	void RenderPlayer();
	void RenderEnemy();
	void RenderBullets();
	void RenderTiles();
	void RenderBuffBody();
	void RenderDebug();
	void RenderDebugGrid();
	void SaveGame(const std::string& filePath);
	void LoadGame(const std::string& filePath);
	void Run();
	void handleEvents();
	void LoadScene(int i);
	bool ValidateCurrentScene();

};

#endif
