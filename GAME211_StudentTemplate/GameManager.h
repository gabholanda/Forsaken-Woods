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

class EnemyBody;
class Bullet;

class GameManager {
private:
	/// These are called "forward declarations" The idea is that a pointer is 
	/// really just an unsigned int, so to create a pointer to it you really only
	/// need to tell the compiler that there is a class called Window and I need
	/// a pointer to it, thus the "class Window*" means trust me, there is a class 
	/// called Window, I just need a pointer to it.

	/// If that was too much for your brain, just #include "Window.h" and declare
	/// Window *ptr and don't use the word "class"  This is a second semester C++
	/// topic anyway
	class Window* windowPtr;
	class Timer* timer;
	bool isRunning;
	bool isDebugging;
	class Scene* currentScene;

	// This might be unfamiliar
	class PlayerBody* player;

	std::vector<Bullet*> bullets;
	std::vector<EnemyBody*> enemies;
	std::vector<Tile*> tiles;

	SpritesheetReader* backgroundReader;
	Uint32 changeSceneEventType;
public:
	GameManager();
	~GameManager();
	bool OnCreate();
	void OnDestroy();


	// These might be unfamiliar
	float getSceneHeight();
	float getSceneWidth();
	Matrix4 getProjectionMatrix();
	PlayerBody* getPlayer() { return player; }
	std::vector<EnemyBody*> getEnemies() { return enemies; }
	std::vector<Bullet*>* getBullets() { return &bullets; }
	std::vector<Tile*>* getTiles() { return &tiles; }
	SpritesheetReader* getBackgroundSpritesheetReader() { return backgroundReader; }
	void RenderPlayer();
	void RenderEnemy();
	void RenderBullets();
	void RenderTiles();
	void RenderDebug();
	SDL_Renderer* getRenderer();

	void Run();
	void handleEvents();
	void LoadScene(int i);
	bool ValidateCurrentScene();

	Uint32 getChangeScene();

};
#endif


