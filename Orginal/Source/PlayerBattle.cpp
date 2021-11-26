#include "PlayerBattle.h"

#include "lib/Input.h"

#include "CommandPlayer.h"
#include "Player.h"

PlayerBattle::PlayerBattle(Player* player) : BattleCharacter(player, StatusData::GetPLStatus(player->GetCharaID()))
{
	// コマンド決定クラスを作成
	mCommand = std::make_unique<CommandPlayer>();

	// アイテムのインベントリの参照を保存
	mInventory = *player->GetInventory();
}

void PlayerBattle::Initialize()
{	
	SetMotion(Character::IDLE);
}
