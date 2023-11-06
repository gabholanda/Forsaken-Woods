#include "ReloadSpeedBuff.h"
#include "PlayerBody.h"

void ReloadSpeedBuff::ApplyBuff(PlayerBody* player)
{
	player->GetGun()->SetReloadSpeed(player->GetGun()->GetReloadSpeed() + reloadSpeedIncrement);

}
