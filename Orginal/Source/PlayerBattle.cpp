#include "PlayerBattle.h"

#include "lib/Input.h"

#include "CommandPlayer.h"
#include "Player.h"

PlayerBattle::PlayerBattle(Player* player) : BattleCharacter(player, StatusData::GetPLStatus(player->GetCharaID()))
{
	mCommand = std::make_unique<CommandPlayer>();
	mInventory = *player->GetInventory();
}

void PlayerBattle::Initialize()
{	
	SetMotion(Character::IDLE);
}
