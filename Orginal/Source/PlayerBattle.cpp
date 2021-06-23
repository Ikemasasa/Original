#include "PlayerBattle.h"

#include "lib/Input.h"

#include "CommandPlayer.h"
#include "DataBase.h"
#include "Player.h"
#include "Singleton.h"

PlayerBattle::PlayerBattle(Player* player) : BattleActor(player, Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(player->GetCharaID()))
{
	mCommand = std::make_unique<CommandPlayer>();
	mInventory = player->GetInventory();
}

void PlayerBattle::Initialize()
{	
	SetMotion(SkinnedMesh::IDLE);
}
