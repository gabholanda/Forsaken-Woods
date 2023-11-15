#include "FireRateBuff.h"
#include "PlayerBody.h"
#include "Gun.h"

void FireRateBuff::ApplyBuff(PlayerBody* player)
{
	player->GetGun()->SetFireRate(player->GetGun()->GetFireRate() + fireRateIncrement);
}