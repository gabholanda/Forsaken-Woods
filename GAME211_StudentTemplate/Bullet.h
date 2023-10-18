#pragma once
#include "Body.h"

class Gun;

class Bullet :
	public Body
{

protected:
	class GameManager* game;
	Vec3 direction;
	Gun* owningGun;
	float lifeTime;

public:
	Bullet(
		Gun* owningGun_,
		Vec3 pos_, Vec3 vel_, Vec3 accel_,
		Vec3 size_,
		float mass_,
		float orientation_,
		float rotation_,
		float angular_,
		float movementSpeed_,
		float scale_,
		float lifeTime_,
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
		owningGun = owningGun_;
		lifeTime = lifeTime_;
	}

	float GetLifeTime() const { return lifeTime; }
	Vec3 GetDirection() const { return direction; }
	void SetDirection(Vec3 direction_) { direction = direction_; }
	bool OnCreate();
	void Render();
	void Update(float deltaTime);
	void setTexture(SDL_Texture* texture_) { texture = texture_; }
};

