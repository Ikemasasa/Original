#include "BattleCharacter.h"

#include "BattleCharacterManager.h"
#include "CommandPlayer.h"

BattleCharacter::BattleCharacter(const Character* chara, Status status) : Character(chara, chara->GetType())
{
	mStatus = status;
}

void BattleCharacter::Update(const BattleCharacterManager* bcm)
{
	mCommand->Update(bcm);

	UpdateWorld();
}

void BattleCharacter::RenderCommand() const
{
	mCommand->Render();
}
