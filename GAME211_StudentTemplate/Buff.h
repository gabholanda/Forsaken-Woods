#pragma once
#include "Body.h"
#include "PlayerBody.h"
class Buff : public Body
{

protected:
    class GameManager* game;

public:
    Buff() : Body{}
    {
        game = nullptr;
    }
	Buff(
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
	{}
	virtual void ApplyBuff(PlayerBody* player);
};

