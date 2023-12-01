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
#include <string>
struct PlayerSaveState {
	float playerHp;
	float maxPlayerHP;
	float movementSpeed;
	float dashCooldown;
};

enum Direction {
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT
};

using namespace std;

class PlayerBody : public Body
{
protected:
	class GameManager* game;
	class Gun* gun;
	bool isDashing = false;
	bool canDash = true;
	bool isMouseButtonDown;
	bool releasedDuringDash;
	bool isMoving;
	float dashMultiplier = 5;
	float dashCooldown = 3;
	float dashLength = 0.15;
	SDL_TimerID timerID;
	Direction dashDirection;
	float initialDashVelX;
	float initialDashVelY;
	float playerHp = 100;
	float maxPlayerHP;
	static PlayerSaveState playerSaveState;
	float playerSpawnIndex;

public:
	PlayerBody() : Body{}
	{
		gun = nullptr;
		game = nullptr;
		isDashing = false;
		canDash = true;
		releasedDuringDash = false;
		isMoving = false;
		dashMultiplier = 3.0f;
		dashCooldown = 3.0f;
		dashLength = 0.3f;
		timerID = 0;
		dashDirection = Direction::NONE;
		initialDashVelX = 0.0f;
		initialDashVelY = 0.0f;
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
		GameManager* game_,
		float playerHP_
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
		playerHp = playerHP_;
		maxPlayerHP = playerHp;
		gun = gun_;
	}


	// use the base class versions of getters

	bool OnCreate();
	void Render();
	void HandleEvents(const SDL_Event& event);
	void Update(float deltaTime);
	void setTexture(SDL_Texture* texture_) { texture = texture_; }
	virtual float getDashCooldown() { return dashCooldown; }
	virtual void setDashCooldown(float dashCooldown_) { dashCooldown = dashCooldown_; }
	Gun* GetGun() const { return gun; }
	GameManager* GetGame() const { return game; }
	virtual void SetGun(Gun* gun_) { gun = gun_; }
	void StartDashTimer();
	void EndDash();
	static Uint32 TimerCallback(Uint32 interval, void* param);
	static Uint32 DashCooldownCallback(Uint32 interval, void* param);
	virtual float getHp() { return playerHp; }
	void setHp(float playerHp_) { playerHp = playerHp_; }
	void setMaxHp(float maxPlayerHp_) { maxPlayerHP = maxPlayerHp_; }
	virtual float getMaxHp() { return maxPlayerHP; }
	virtual float GetPlayerSpawnIndex() { return playerSpawnIndex; }
	void Death();

	const char* Text() const
	{
		std::string text = "HP: " + to_string((int)playerHp) + " / " + to_string((int)maxPlayerHP);

		char* result = new char[text.length() + 1];

		// Copy the contents of nameString to the newly allocated memory using strcpy_s
		strcpy_s(result, text.length() + 1, text.c_str());

		return result;
	}
};

#endif /* PLAYERBODY_H */
