#include "CommandEnemy.h"

#include "lib/Random.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"

void CommandEnemy::Update(const BattleCharacterManager* bcm)
{
	if (IsBehaviourEnable()) return;

	BattleState::GetInstance().SetState(BattleState::State::ENEMY_ATTACK);

	const std::vector<int>& aliveIDs = bcm->GetAliveObjIDs(Character::PLAYER);

	int index = Random::RandomRange(0, aliveIDs.size() - 1);
	mTargetObjIDs.push_back(aliveIDs[index]);

	mBehaviour = Behaviour::ATTACK;
}
