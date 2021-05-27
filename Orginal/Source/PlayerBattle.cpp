#include "PlayerBattle.h"

#include "lib/Input.h"

#include "Player.h"
#include "DataBase.h"
#include "Singleton.h"

PlayerBattle::PlayerBattle(const std::shared_ptr<Actor>& player) : BattleActor(player, Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(player->GetCharaID()))
{
	mCommand = std::make_unique<CommandPlayer>();
}

void PlayerBattle::Initialize()
{	
	SetMotion(SkinnedMesh::IDLE);
}
