//
//  PlayerBody.h
//  DemoAI
//
//  Created by Gail Harris on 2021-Dec-23.
//

#ifndef PLAYERBODY_H
#define PLAYERBODY_H

#include <stdio.h>
#include "Body.h"
#include "GameManager.h"

class PlayerBody : public Body
{
protected:
	class GameManager* game;
	class Gun* gun;

public:
	PlayerBody() : Body{}
	{
		gun = nullptr;
		game = nullptr;
	}

	~PlayerBody();

	// Note the last parameter in this constructor!
	// Look in GameManager.cpp to see how this is called.
	// Look in .cpp file, at Render(), to see why game_ is a parameter.
	PlayerBody(
		Gun* gun_,
		Vec3 pos_, Vec3 vel_, Vec3 accel_,
		Vec3 size_,
		float mass_,
		float orientation_,
		float rotation_,
		float angular_,
		float movementSpeed_,
		float scale_,
		GameManager* game_
	) : Body{
		  pos_
		, vel_
		, accel_
		, size_
		, mass_
		, orientation_
		, rotation_
		, angular_
		, movementSpeed_
		, scale_
	}
		, game{ game_ }

	{
		gun = gun_;
	}


	// use the base class versions of getters

	bool OnCreate();
	void Render();
	void HandleEvents(const SDL_Event& event);
	void Update(float deltaTime);
	void setTexture(SDL_Texture* texture_) { texture = texture_; }
	Gun* GetGun() const { return gun; }
	GameManager* GetGame() const { return game; }
};

#endif /* PLAYERBODY_H */
