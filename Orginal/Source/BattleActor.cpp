#include "BattleActor.h"

#include "BattleActorManager.h"
#include "CommandPlayer.h"

BattleActor::BattleActor(const Actor* actor, Status status) : Actor(actor, actor->GetType())
{
	mStatus = status;
}

void BattleActor::Update(const BattleActorManager* bam)
{
	mCommand->Update(bam);

	UpdateWorld();
}

void BattleActor::RenderCommand() const
{
	mCommand->Render();
}
