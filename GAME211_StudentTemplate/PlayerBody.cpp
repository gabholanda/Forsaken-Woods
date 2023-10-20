//
//  PlayerBody.cpp
//  DemoAI
//
//  Created by Gail Harris on 2021-Dec-23.
//

#include "PlayerBody.h"
#include "RotationUtils.h"
#include "Bullet.h"

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
		switch (event.key.keysym.scancode) {
		case SDL_SCANCODE_W:
			vel.y = movementSpeed;
			break;
		case SDL_SCANCODE_A:
			vel.x = -(movementSpeed);
			break;
		case SDL_SCANCODE_S:
			vel.y = -(movementSpeed);
			break;
		case SDL_SCANCODE_D:
			vel.x = movementSpeed;
			break;
		}
	}

	if (event.type == SDL_KEYUP && event.key.repeat == 0) {
		switch (event.key.keysym.scancode) {
		case SDL_SCANCODE_W:
			vel.y = 0;
			break;
		case SDL_SCANCODE_A:
			vel.x = 0;
			break;
		case SDL_SCANCODE_S:
			vel.y = 0;
			break;
		case SDL_SCANCODE_D:
			vel.x = 0;
			break;
		}
	}

	if (event.type == SDL_MOUSEMOTION)
	{
		RotationUtils::RotateTowardsMouse(orientation, game->getProjectionMatrix(), pos);
	}

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (Gun* gun = game->getPlayer()->GetGun())
		{
			// TODO: throw all of this into the Gun class and allow it to control fire rate
			int x, y;
			SDL_GetMouseState(&x, &y);
			Vec3 mousePos = MMath::inverse(game->getProjectionMatrix()) * Vec3(float(x), float(y), 0.0f);
			Vec3 direction = mousePos - game->getPlayer()->getPos();
			float mass = 1.0f;
			float desiredAngle = std::atan2(direction.y, direction.x);
			float orientation = -desiredAngle;
			float rotation = 0.0f;
			float angular = 0.0f;
			float movementSpeed = 1.0f;
			float scale = 0.5f;
			float lifeTime = 2.f;
			Vec3 size(0.5f, 0.5f, 0.0f);
			Vec3 position = game->getPlayer()->getPos();
			Vec3 velocity(0.0f, 0.0f, 0.0f);
			Vec3 acceleration(0.0f, 0.0f, 0.0f);
			Bullet* bullet = new Bullet(
				gun,
				position,
				velocity,
				acceleration,
				size,
				mass,
				orientation,
				rotation,
				angular,
				movementSpeed,
				scale,
				lifeTime,
				game);
			bullet->OnCreate();
			bullet->SetDirection(direction);
			game->getBullets()->push_back(bullet);
		}
	}

	if (VMath::mag(vel) > VERY_SMALL) {
		vel = VMath::normalize(vel) * movementSpeed;
	}
	// etc
}

void PlayerBody::Update(float deltaTime)
{
	// Update position, call Update from base class
	// Note that would update velocity too, and rotation motion

	Body::Update(deltaTime);

}