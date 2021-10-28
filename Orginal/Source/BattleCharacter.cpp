#include "BattleCharacter.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CommandPlayer.h"

BattleCharacter::BattleCharacter(const Character* chara, Status status) : Character(chara)
{
	mStatus = status;
	mBoneCollision.enable = false;
}

void BattleCharacter::Update(const BattleCharacterManager* bcm)
{
	// �J�n���o���Ȃ�R�}���h���삵�Ȃ�
	if (BattleState::GetInstance().GetState() != BattleState::State::BEGIN)
	{
		mCommand->Update(bcm);
	}

	UpdateWorld();
}

void BattleCharacter::RenderCommand() const
{
	// �J�n���o������Ȃ��Ȃ�`��
	if (BattleState::GetInstance().GetState() != BattleState::State::BEGIN)
	{
		mCommand->Render();
	}
}
