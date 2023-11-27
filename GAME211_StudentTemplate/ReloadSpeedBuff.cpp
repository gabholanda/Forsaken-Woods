#include "ReloadSpeedBuff.h"
#include "PlayerBody.h"
#include "Gun.h"

void ReloadSpeedBuff::ApplyBuff(PlayerBody* player)
{
	player->GetGun()->SetReloadSpeed(player->GetGun()->GetReloadSpeed() - reloadSpeedIncrement);

}
