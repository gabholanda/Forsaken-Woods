#ifndef SCENE1_H
#define SCENE1_H

#include "Scene.h"
#include "Randomizer.h"
#include "Gun.h"
#include "Pistol.h"


using namespace MATH;
class Scene1 : public Scene {
private:
	float xAxis;	// scene width, in game coords, set in constructor
	float yAxis;	// scene height, in game coords, set in constructor
	SDL_Window* window;		// an SDL window with a SDL renderer
	SDL_Renderer* renderer;	// the renderer associated with SDL window
	Matrix4 projectionMatrix;	// set in OnCreate()
	Matrix4     inverseProjection;	// set in OnCreate()
	Randomizer* randomizer;
	std::vector<int> bulletsToDestroy;
	

public:
	// This constructor may be different from what you've seen before
	// Notice the second parameter, and look in GameManager.cpp
	// to see how this constructor is called.
	Scene1(SDL_Window* sdlWindow, GameManager* game_);
	~Scene1();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void PostRenderUpdate(const float time);
	void HandleEvents(const SDL_Event& event);
	float getxAxis() { return xAxis; }
	float getyAxis() { return yAxis; }
	SDL_Window* getWindow() { return window; }
	Matrix4 getProjectionMatrix() { return camera->getProjectionMatrix(); }
	Matrix4 getInverseMatrix() { return inverseProjection; }
};

#endif
