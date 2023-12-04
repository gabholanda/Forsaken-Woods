//
//  PlayerBody.cpp
//  DemoAI
//
//  Created by Gail Harris on 2021-Dec-23.
//

#include "PlayerBody.h"
#include "RotationUtils.h"
#include "Gun.h"


PlayerSaveState PlayerBody::playerSaveState;

PlayerBody::~PlayerBody()
{
	delete gun;
}

bool PlayerBody::OnCreate()
{
	image = IMG_Load("Rogue.png");
	SDL_Renderer* renderer = game->getRenderer();
	texture = SDL_CreateTextureFromSurface(renderer, image);
	if (image == nullptr) {
		std::cerr << "Can't open the image" << std::endl;
		return false;
	}
	return true;
}

void PlayerBody::Render()
{
	// This is why we need game in the constructor, to get the renderer, etc.
	SDL_Renderer* renderer = game->getRenderer();
	Matrix4 projectionMatrix = game->getProjectionMatrix();

	// square represents the position and dimensions for where to draw the image
	SDL_Rect square;
	Vec3 screenCoords;
	float    w, h;

	// convert the position from game coords to screen coords.
	screenCoords = projectionMatrix * pos;

	// Scale the image, in case the .png file is too big or small
	w = image->w * scale;
	h = image->h * scale;

	// The square's x and y values represent the top left corner of 
	// where SDL will draw the .png image.
	// The 0.5f * w/h offset is to place the .png so that pos represents the center
	// (Note the y axis for screen coords points downward, hence subtraction!!!!)
	square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	square.y = static_cast<int>(screenCoords.y - 0.5f * h);
	square.w = static_cast<int>(w);
	square.h = static_cast<int>(h);

	// Convert character orientation from radians to degrees.
	float orientationDegrees = orientation * 180.0f / M_PI;

	SDL_RenderCopyEx(renderer, texture, nullptr, &square,
		orientationDegrees, nullptr, SDL_FLIP_NONE);
}

	void PlayerBody::HandleEvents(const SDL_Event& event)
	{
		if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
				isBeginningOfGame = false;
		}

		if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {

			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_W:
				vel.y = movementSpeed; // Move down
				break;
			case SDL_SCANCODE_A:
				vel.x = -(movementSpeed); // Move left
				break;
			case SDL_SCANCODE_S:
				vel.y = -(movementSpeed); // Move up
				break;
			case SDL_SCANCODE_D:
				vel.x = movementSpeed; // Move right
				break;
			}
		}

		if (VMath::mag(vel) > VERY_SMALL && !isDashing) {
			vel = VMath::normalize(vel) * movementSpeed;
		}

		
		if (event.type == SDL_KEYUP && event.key.repeat == 0 && !isDashing) {
			
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_W:
				if (vel.y > 0) {
					vel.y = 0; // Stop moving up only if the key was released and the entity is moving up
				}
				break;
			case SDL_SCANCODE_A:
				if (vel.x < 0) {
					vel.x = 0; // Stop moving left only if the key was released and the entity is moving left
				}
				break;
			case SDL_SCANCODE_S:
				if (vel.y < 0) {
					vel.y = 0; // Stop moving down only if the key was released and the entity is moving down
				}
				break;
			case SDL_SCANCODE_D:
				if (vel.x > 0) {
					vel.x = 0; // Stop moving right only if the key was released and the entity is moving right
				}
				break;
			}
		}
		if (event.type == SDL_KEYUP && isDashing) {
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_W:
				if (dashDirection == UP || dashDirection == UP_LEFT || dashDirection == UP_RIGHT) {
					releasedDuringDash = true;
				}
				break;
			case SDL_SCANCODE_A:
				if (dashDirection == LEFT || dashDirection == UP_LEFT || dashDirection == DOWN_LEFT) {
					releasedDuringDash = true;
				}
				break;
			case SDL_SCANCODE_S:
				if (dashDirection == DOWN || dashDirection == DOWN_LEFT || dashDirection == DOWN_RIGHT) {
					releasedDuringDash = true;
				}
				break;
			case SDL_SCANCODE_D:
				if (dashDirection == RIGHT || dashDirection == UP_RIGHT || dashDirection == DOWN_RIGHT) {
					releasedDuringDash = true;
				}
				break;
			}
		}

		if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
			if (!isDashing && canDash) {
				isDashing = true;
				float diagonalDashSpeed = movementSpeed * dashMultiplier / sqrt(2.0f); // Adjusted diagonal dash speed
				if (vel.x > 0 && vel.y > 0) {
					// initiate diagonal dash down-right
					dashDirection = DOWN_RIGHT;
					initialDashVelX = diagonalDashSpeed;
					initialDashVelY = diagonalDashSpeed;
				}
				else if (vel.x < 0 && vel.y > 0) {
					// initiate diagonal dash down-left
					dashDirection = DOWN_LEFT;
					initialDashVelX = -diagonalDashSpeed;
					initialDashVelY = diagonalDashSpeed;
				}
				else if (vel.x > 0 && vel.y < 0) {
					// initiate diagonal dash up-right
					dashDirection = UP_RIGHT;
					initialDashVelX = diagonalDashSpeed;
					initialDashVelY = -diagonalDashSpeed;
				}
				else if (vel.x < 0 && vel.y < 0) {
					// initiate diagonal dash up-left
					dashDirection = UP_LEFT;
					initialDashVelX = -diagonalDashSpeed;
					initialDashVelY = -diagonalDashSpeed;
				}
				else {
					// handle cardinal direction dash as before
					if (vel.x > 0) {
						dashDirection = RIGHT;
						initialDashVelX = movementSpeed * dashMultiplier;
						initialDashVelY = 0;
					}
					else if (vel.x < 0) {
						dashDirection = LEFT;
						initialDashVelX = -movementSpeed * dashMultiplier;
						initialDashVelY = 0;
					}
					else if (vel.y > 0) {
						dashDirection = UP;
						initialDashVelY = movementSpeed * dashMultiplier;
						initialDashVelX = 0;
					}
					else if (vel.y < 0) {
						dashDirection = DOWN;
						initialDashVelY = -movementSpeed * dashMultiplier;
						initialDashVelX = 0;
					}
				}
				// Apply initial dash velocity
				vel.x = initialDashVelX;
				vel.y = initialDashVelY;
				StartDashTimer();
				canDash = false;
			}
		}


			if (event.type == SDL_MOUSEMOTION)
			{
				RotationUtils::RotateTowardsMouse(orientation, game->getProjectionMatrix(), pos);
			}

			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
			{
				if (gun)
				{
					if (gun->getCurrentAmmo() > 0 && !gun->getIsReloading()) {
						
						gun->Shoot();

					}
					else {
						if (!gun->getIsReloading()) {
							gun->Reload();
						}
					}

				}


			}

			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
				if (!gun->getIsReloading()) {
					gun->Reload();
				}
			}
	}

	void PlayerBody::StartDashTimer() {
		timerID = SDL_AddTimer(static_cast<Uint32>(dashLength * 1000.0f), TimerCallback, this);
		timerID = SDL_AddTimer(static_cast<Uint32>(dashCooldown * 1000.0f), DashCooldownCallback, this);
	}

	void PlayerBody::EndDash() {
		isDashing = false;
		if (dashDirection == UP || dashDirection == DOWN) {
			vel.y /= dashMultiplier;
			if (releasedDuringDash) {
				vel.y = 0;
				releasedDuringDash = false;
			}

		}
		else if (dashDirection == LEFT || dashDirection == RIGHT) {
			vel.x /= dashMultiplier;
			if (releasedDuringDash) {
				vel.x = 0;
				releasedDuringDash = false;
			}
		}
		else if (dashDirection == UP_LEFT || dashDirection == UP_RIGHT || dashDirection == DOWN_LEFT || dashDirection ==DOWN_RIGHT) {
			vel.x /= dashMultiplier;
			vel.y /= dashMultiplier;
			if (releasedDuringDash) {
				vel.x = 0;
				vel.y = 0;
				releasedDuringDash = false;
			}
		}
		dashDirection = NONE;
	}

	Uint32 PlayerBody::TimerCallback(Uint32 interval, void* param) {
		// Reset the velocity to its original value
		PlayerBody* player = static_cast<PlayerBody*>(param);
		player->EndDash();
		return 0; // Timer will not repeat
	}

	Uint32 PlayerBody::DashCooldownCallback(Uint32 interval, void* param) {
		PlayerBody* player = static_cast<PlayerBody*>(param);
		player->canDash = true;
		return 0; // Timer will not repeat
	}



	void PlayerBody::Death()
	{
		Mix_PlayChannel(-1, game->getDeathSound(), 0);
		game->SetRestart(true);

	}


void PlayerBody::Update(float deltaTime)
{
	Body::Update(deltaTime);
}