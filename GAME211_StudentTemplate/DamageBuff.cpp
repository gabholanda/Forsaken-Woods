#include "DamageBuff.h"
#include "Gun.h"
#include "PlayerBody.h"



void DamageBuff::ApplyBuff(PlayerBody* player)
{
	player->GetGun()->SetDamage(player->GetGun()->GetDamage() + damageIncrement);
}
		