#include "HealthBuff.h"
#include "PlayerBody.h"


void HealthBuff::ApplyBuff(PlayerBody* player)
{
	player->setMaxHp(player->getMaxHp() + healthIncrement);
}
