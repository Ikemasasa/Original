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
	// 開始演出中ならコマンド操作しない
	if (BattleState::GetInstance().GetState() != BattleState::State::BEGIN)
	{
		mCommand->Update(bcm);
	}

	UpdateWorld();
}

void BattleCharacter::RenderCommand() const
{
	// 開始演出中じゃないなら描画
	if (BattleState::GetInstance().GetState() != BattleState::State::BEGIN)
	{
		mCommand->Render();
	}
}
