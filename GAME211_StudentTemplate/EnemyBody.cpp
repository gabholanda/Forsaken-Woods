#include "EnemyBody.h"
#include "RotationUtils.h"
#include "Gun.h"

EnemyBody::~EnemyBody()
{
	delete gun;
}

bool EnemyBody::OnCreate()
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


void EnemyBody::Render()
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

void EnemyBody::Update(float deltaTime)
{
	// Update position, call Update from base class
	// Note that would update velocity too, and rotation motion

	Body::Update(deltaTime);

}

void EnemyBody::MoveTowardsPlayer(float deltaTime, PlayerBody* target)
{
	RotationUtils::RotateTowardsTarget(orientation, target->getPos(), pos);
	Vec3 direction = VMath::normalize(target->getPos() - pos);
	pos.x += direction.x * movementSpeed * deltaTime;
	pos.y += direction.y * movementSpeed * deltaTime;

	
}

void EnemyBody::RangeAttack(PlayerBody* target)
{
	if (gun)
	{
		if (VMath::distance(pos, target->getPos()) <= 10)
		{
			gun->ShootForEnemy(target);
		}
	}
}

void EnemyBody::dealDamageForEnemy(float damage, float playerHp)
{
	playerHp -= damage;
}
