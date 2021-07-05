#include "CommandEnemy.h"

#include "BattleActorManager.h"

void CommandEnemy::Update(const BattleActorManager* bam)
{
	if (IsBehaviourEnable()) return;

	const std::vector<int>& aliveIDs = bam->GetAliveActorIDs(Actor::PLAYER);

	int index = rand() % aliveIDs.size();
	mTargetObjID = aliveIDs[index];

	mBehaviour = Behaviour::ATTACK;
}
