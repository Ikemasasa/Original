#include "CommandEnemy.h"

#include "BattleActorManager.h"

void CommandEnemy::Update(const BattleActorManager* bam)
{
	const std::vector<int>& objectIDs = bam->GetObjectIDs(Actor::PLAYER);

	int index = rand() % objectIDs.size();
	mTargetObjID = objectIDs[index];

	mBehaviour = Behaviour::ATTACK;
}
