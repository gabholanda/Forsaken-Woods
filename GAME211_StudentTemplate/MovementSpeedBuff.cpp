#include "MovementSpeedBuff.h"
#include "PlayerBody.h"


void MovementSpeedBuff::ApplyBuff(PlayerBody* player)
{
	player->setMovementSpeed(player->getMovementSpeed() + movementSpeedIncrement);
}
