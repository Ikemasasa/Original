#include "CommandBossEnemy.h"

#include <random>

#include "lib/Random.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "StatusData.h"


CommandBossEnemy::CommandBossEnemy() : CommandBase()
{

}

CommandBossEnemy::~CommandBossEnemy()
{

}

void CommandBossEnemy::Update(const BattleCharacterManager* bcm)
{
	// behaviour決定していたら return
	if (IsBehaviourEnable()) return;

	// 1体と2体を交互にする
	// 全員の中でHPが低いやつを狙う
	// (次はHPが低い奴を狙いやすくする)

	// ターゲット数を決める
	int targetNum = 0;
	switch (mState)
	{
	case State::TARGET_ONE: 
		targetNum = 1;
		mState = State::TARGET_TWO; // 次は2体攻撃
		break;

	case State::TARGET_TWO: // 2体攻撃
		targetNum = 2;
		mState = State::TARGET_ONE; // 次は1体攻撃
		break;
	}

	// targetNumを生きてるプレイヤーの数以下にする
	int maxNum = bcm->GetAliveObjIDs(Character::PLAYER).size();
	targetNum = Math::Min(targetNum, maxNum);

	// ターゲット取得
	int rnd = Random::Rand() % 101;
	if (rnd <= ATTACK_LOWHP_CHARA_PERCENT)
	{
		// 低HPのキャラを狙う
		mTargetObjIDs = SearchLowHPPlayer(targetNum, bcm);
	}
	else
	{
		// ランダムに狙う
		mTargetObjIDs = GetRandomPlayerObjIDs(targetNum, bcm);
	}

	BattleState::GetInstance().SetState(BattleState::State::ENEMY_ATTACK);
	mBehaviour = Behaviour::ATTACK;
}


std::vector<int> CommandBossEnemy::SearchLowHPPlayer(const size_t num, const BattleCharacterManager* bcm)
{
	const std::vector<int>& aliveObjIDs = bcm->GetAliveObjIDs(Character::PLAYER);
	const std::vector<std::shared_ptr<BattleCharacter>>& bCharacters = bcm->GetBCharacters();

	// 現在の低HPのキャラを保存する変数( -1で初期化
	std::vector<int> currentObjIDs(num, -1);

	size_t aliveNum = aliveObjIDs.size();
	for (auto& aliveObjID : aliveObjIDs)
	{
		bool isContinue = false;
		for (size_t i = 0; i < num; ++i)
		{
			// 未登録なら登録する
			if (currentObjIDs[i] == -1)
			{
				isContinue = true;
				currentObjIDs[i] = aliveObjID;
				break;
			}
		}
		if (isContinue) continue;

		// HP取得
		std::vector<int> currentCharaHPs(num, 0);
		for (size_t i = 0; i < num; ++i)
		{
			int charaID = bCharacters[currentObjIDs[i]]->GetCharaID();
			currentCharaHPs[i] = StatusData::GetPLStatus(charaID).GetHP();
		}
		int nextCharaHP = StatusData::GetPLStatus(bCharacters[aliveObjID]->GetCharaID()).GetHP();

		// 今の最小値と比べて、変更があるかチェック
		int changeIndex = -1;
		int subMin = 0;
		for (size_t i = 0; i < num; ++i)
		{
			int sub = nextCharaHP - currentCharaHPs[i];
			if (sub < subMin)
			{
				subMin = sub;
				changeIndex = i;
			}
		}

		// 変更するなら
		if (changeIndex != -1)
		{
			currentObjIDs[changeIndex] = aliveObjID;
		}

	}


	return currentObjIDs;
}

std::vector<int> CommandBossEnemy::GetRandomPlayerObjIDs(const size_t num, const BattleCharacterManager* bcm)
{
	// 生きてるプレイヤーのobjidを取得
	std::vector<int> aliveObjIDs = bcm->GetAliveObjIDs(Character::Type::PLAYER);

	std::random_device rnd;
	std::mt19937 mt(rnd());

	std::shuffle(aliveObjIDs.begin(), aliveObjIDs.end(), mt);

	std::vector<int> ret(num);
	for (size_t i = 0; i < num; ++i)
	{
		ret[i] = aliveObjIDs[i];
	}
	
	return ret;
}
