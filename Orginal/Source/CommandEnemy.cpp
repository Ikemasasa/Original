#include "CommandEnemy.h"

#include "BattleCharacterManager.h"

void CommandEnemy::Update(const BattleCharacterManager* bcm)
{
	if (IsBehaviourEnable()) return;

	const std::vector<int>& aliveIDs = bcm->GetAliveCharaIDs(Character::PLAYER);

	int index = rand() % aliveIDs.size();
	mTargetObjID = aliveIDs[index];

	mBehaviour = Behaviour::ATTACK;
}
