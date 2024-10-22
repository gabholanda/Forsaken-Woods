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
#include "BuffManager.h"
#include <SDL_mixer.h>

class EnemyBody;
class Buff;
class Bullet;
class Grid;
class UIText;

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
	std::vector<Bullet*> bullets;
	std::vector<Bullet*> enemyBullets;
	std::vector<EnemyBody*> enemies;
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
	Mix_Music* backgroundMusic = NULL;
	Mix_Chunk* shootSoundEffect = NULL;
	Mix_Chunk* buffSoundEffect = NULL;
	Mix_Chunk* deathSoundEffect = NULL;
	Mix_Chunk* winSoundEffect = NULL;
	SpritesheetReader* backgroundReader;
	SpritesheetReader* treeReader;
	SpritesheetReader* flowerReader;
	SpritesheetReader* insideTreeReader;
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
	bool LoadSounds();
	// These might be unfamiliar
	float getSceneHeight();
	float getSceneWidth();
	Matrix4 getProjectionMatrix();
	void InitializeController();

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
	Mix_Chunk* getShootSound() { return shootSoundEffect; }
	Mix_Chunk* getBuffSound() { return buffSoundEffect; }
	Mix_Chunk* getDeathSound() { return deathSoundEffect; }
	Mix_Chunk* getWinSound() { return winSoundEffect; }
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

	void Run();
	void handleEvents();
	void LoadScene(int i);
	bool ValidateCurrentScene();

};

#endif
