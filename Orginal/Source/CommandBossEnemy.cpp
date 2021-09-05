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
	// behaviour���肵�Ă����� return
	if (IsBehaviourEnable()) return;

	// 1�̂�2�̂����݂ɂ���
	// �S���̒���HP���Ⴂ���_��
	// (����HP���Ⴂ�z��_���₷������)

	// �^�[�Q�b�g�������߂�
	int targetNum = 0;
	switch (mState)
	{
	case State::TARGET_ONE: 
		targetNum = 1;
		mState = State::TARGET_TWO; // ����2�̍U��
		break;

	case State::TARGET_TWO: // 2�̍U��
		targetNum = 2;
		mState = State::TARGET_ONE; // ����1�̍U��
		break;
	}

	// targetNum�𐶂��Ă�v���C���[�̐��ȉ��ɂ���
	int maxNum = bcm->GetAliveObjIDs(Character::PLAYER).size();
	targetNum = Math::Min(targetNum, maxNum);

	// �^�[�Q�b�g�擾
	int rnd = Random::Rand() % 101;
	if (rnd <= ATTACK_LOWHP_CHARA_PERCENT)
	{
		// ��HP�̃L������_��
		mTargetObjIDs = SearchLowHPPlayer(targetNum, bcm);
	}
	else
	{
		// �����_���ɑ_��
		mTargetObjIDs = GetRandomPlayerObjIDs(targetNum, bcm);
	}

	BattleState::GetInstance().SetState(BattleState::State::ENEMY_ATTACK);
	mBehaviour = Behaviour::ATTACK;
}


std::vector<int> CommandBossEnemy::SearchLowHPPlayer(const size_t num, const BattleCharacterManager* bcm)
{
	const std::vector<int>& aliveObjIDs = bcm->GetAliveObjIDs(Character::PLAYER);
	const std::vector<std::shared_ptr<BattleCharacter>>& bCharacters = bcm->GetBCharacters();

	// ���݂̒�HP�̃L������ۑ�����ϐ�( -1�ŏ�����
	std::vector<int> currentObjIDs(num, -1);

	size_t aliveNum = aliveObjIDs.size();
	for (auto& aliveObjID : aliveObjIDs)
	{
		bool isContinue = false;
		for (size_t i = 0; i < num; ++i)
		{
			// ���o�^�Ȃ�o�^����
			if (currentObjIDs[i] == -1)
			{
				isContinue = true;
				currentObjIDs[i] = aliveObjID;
				break;
			}
		}
		if (isContinue) continue;

		// HP�擾
		std::vector<int> currentCharaHPs(num, 0);
		for (size_t i = 0; i < num; ++i)
		{
			int charaID = bCharacters[currentObjIDs[i]]->GetCharaID();
			currentCharaHPs[i] = StatusData::GetPLStatus(charaID).GetHP();
		}
		int nextCharaHP = StatusData::GetPLStatus(bCharacters[aliveObjID]->GetCharaID()).GetHP();

		// ���̍ŏ��l�Ɣ�ׂāA�ύX�����邩�`�F�b�N
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

		// �ύX����Ȃ�
		if (changeIndex != -1)
		{
			currentObjIDs[changeIndex] = aliveObjID;
		}

	}


	return currentObjIDs;
}

std::vector<int> CommandBossEnemy::GetRandomPlayerObjIDs(const size_t num, const BattleCharacterManager* bcm)
{
	// �����Ă�v���C���[��objid���擾
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
