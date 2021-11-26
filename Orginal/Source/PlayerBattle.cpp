#include "PlayerBattle.h"

#include "lib/Input.h"

#include "CommandPlayer.h"
#include "Player.h"

PlayerBattle::PlayerBattle(Player* player) : BattleCharacter(player, StatusData::GetPLStatus(player->GetCharaID()))
{
	// �R�}���h����N���X���쐬
	mCommand = std::make_unique<CommandPlayer>();

	// �A�C�e���̃C���x���g���̎Q�Ƃ�ۑ�
	mInventory = *player->GetInventory();
}

void PlayerBattle::Initialize()
{	
	SetMotion(Character::IDLE);
}
