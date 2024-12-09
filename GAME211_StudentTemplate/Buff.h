#pragma once
#include "Body.h"
class PlayerBody;

class Buff : public Body
{

protected:
    class GameManager* game;
	const char* name;
	bool canCollect = false;


public:
	Buff(const char* name_)
	{
		name = name_;
	
	}

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
	void Render();
	bool OnCreate();
	void setTexture(SDL_Texture* texture_) { texture = texture_; }
	void setCanCollect(bool canCollect_);
	bool getCanCollect() {return canCollect;}
	std::string getName() const;  // Pure virtual function for buff name
	const char* Text() const {
		std::string nameString = std::string(name);
		char* result = new char[nameString.length() + 1];

		strcpy_s(result, nameString.length() + 1, nameString.c_str());

		return result;
	}

};

