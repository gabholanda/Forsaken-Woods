#include "DashCooldownBuff.h"
#include "PlayerBody.h"

void DashCooldownBuff::ApplyBuff(PlayerBody* player)
{
	player->setDashCooldown(player->getDashCooldown() - dashCooldownIncrement);
}
