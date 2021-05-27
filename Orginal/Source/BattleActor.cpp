#include "BattleActor.h"

#include "BattleActorManager.h"
#include "CommandPlayer.h"

BattleActor::BattleActor(const std::shared_ptr<Actor>& actor, Status status) : Actor(actor, actor->GetType())
{
	mStatus = status;
}

bool BattleActor::Update(const BattleActorManager* bam)
{
	mCommand->Update(bam);

	UpdateWorld();

	return mCommand->IsBehaviourEnable();
}