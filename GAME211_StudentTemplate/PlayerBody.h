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
		float dashMultiplier = 3;
		float dashCooldown = 3;
		float dashLength = 0.3;
		SDL_TimerID timerID;
		Direction dashDirection;
		float initialDashVelX; 
		float initialDashVelY;

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
		virtual float getDashCooldown() { return dashCooldown; }
		virtual void setDashCooldown(float dashCooldown_) { dashCooldown = dashCooldown_; }
		Gun* GetGun() const { return gun; }
		GameManager* GetGame() const { return game; }
		void StartDashTimer();
		void EndDash();
		static Uint32 TimerCallback(Uint32 interval, void* param);
		static Uint32 DashCooldownCallback(Uint32 interval, void* param);
	};

	#endif /* PLAYERBODY_H */
