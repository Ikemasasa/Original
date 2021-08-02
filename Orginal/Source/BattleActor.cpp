#include "BattleActor.h"

#include "BattleActorManager.h"
#include "CommandPlayer.h"

BattleCharacter::BattleCharacter(const Character* chara, Status status) : Character(chara, chara->GetType())
{
	mStatus = status;
}

void BattleCharacter::Update(const BattleActorManager* bam)
{
	mCommand->Update(bam);

	UpdateWorld();
}

void BattleCharacter::RenderCommand() const
{
	mCommand->Render();
}
