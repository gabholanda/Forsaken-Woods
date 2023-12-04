#include "RotationUtils.h"

void RotationUtils::RotateTowardsMouse(float& outOrientation, Matrix4 projectionMatrix, Vec3 pos)
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	Vec3 mousePos = MMath::inverse(projectionMatrix) * Vec3(float(x), float(y), 0.0f);
	Vec3 direction = VMath::normalize(mousePos - pos);
	float desiredAngle = std::atan2(direction.y, direction.x);
	float newOrientation = -desiredAngle;
	outOrientation = newOrientation;
}

void RotationUtils::RotateTowardsTarget(float& outOrietation, Vec3 targetPos, Vec3 pos)
{
	if (VMath::distance(targetPos, pos) <= VERY_SMALL)
	{
		return;
	}
	Vec3 direction = VMath::normalize(targetPos - pos);
	float desiredAngle = std::atan2(direction.y, direction.x);
	float newOrientation = -desiredAngle;
	outOrietation = newOrientation;
}
