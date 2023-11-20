#ifndef ENEMYBODY_H
#define ENEMYBODY_H

#pragma once
#include <stdio.h>
#include "Body.h"
#include "GameManager.h"
#include "PlayerBody.h"
class EnemyBody : public Body
{
protected:
	class GameManager* game;
	class Gun* gun;
	float enemyHp = 100;

public:
	EnemyBody() : Body{}
	{
		game = nullptr;
		gun = nullptr;
	}

	// Note the last parameter in this constructor!
	// Look in GameManager.cpp to see how this is called.
	// Look in .cpp file, at Render(), to see why game_ is a parameter.
	EnemyBody(
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
		float enemyHp_
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
	void setPosition(Vec3 newPos) {
		pos = newPos;
	}

	~EnemyBody();

	bool OnCreate();
	void Render();
	void Update(float deltaTime);
	void setTexture(SDL_Texture* texture_) { texture = texture_; }
	void MoveTowardsPlayer(float deltaTime, PlayerBody* target);
	GameManager* GetGame() const { return game; }
	void RangeAttack(PlayerBody* target);
	virtual float getHp() { return enemyHp; }
	void dealDamageForEnemy(float damage, float playerHp);
	void setHp(float enemyHp_) { enemyHp = enemyHp_; }
	Gun* GetGun() const { return gun; }

};

#endif