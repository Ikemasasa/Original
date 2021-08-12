#include "CommandEnemy.h"

#include "lib/Random.h"

#include "BattleCharacterManager.h"

void CommandEnemy::Update(const BattleCharacterManager* bcm)
{
	if (IsBehaviourEnable()) return;

	const std::vector<int>& aliveIDs = bcm->GetAliveCharaIDs(Character::PLAYER);

	int index = Random::RandomRange(0, aliveIDs.size() - 1);
	mTargetObjIDs.push_back(aliveIDs[index]);

	mBehaviour = Behaviour::ATTACK;
}
