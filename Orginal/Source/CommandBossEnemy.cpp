#include "CommandBossEnemy.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "Singleton.h"
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

	// ��HP�̃L������targetNum���擾���Ă��ă^�[�Q�b�g�ɂ���
	std::vector<int> objIDs = SearchLowHPChara(targetNum, bcm);
	for (auto& objID : objIDs)
	{
		mTargetObjIDs.push_back(objID);
	}

	BattleState::GetInstance().SetState(BattleState::State::ENEMY_ATTACK);
	mBehaviour = Behaviour::ATTACK;
}


std::vector<int> CommandBossEnemy::SearchLowHPChara(const int num, const BattleCharacterManager* bcm)
{
	const std::vector<int>& aliveObjIDs = bcm->GetAliveObjIDs(Character::PLAYER);
	const std::vector<std::shared_ptr<BattleCharacter>>& bCharacters = bcm->GetBCharacters();
	if (aliveObjIDs.size() < num) return aliveObjIDs;

	// ���݂̒�HP�̃L������ۑ�����ϐ�( -1�ŏ�����
	std::vector<int> currentObjIDs(num, -1);

	size_t aliveNum = aliveObjIDs.size();
	for (auto& aliveObjID : aliveObjIDs)
	{
		bool isContinue = false;
		for (int i = 0; i < num; ++i)
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
		for (int i = 0; i < num; ++i)
		{
			int charaID = bCharacters[currentObjIDs[i]]->GetCharaID();
			currentCharaHPs[i] = Singleton<StatusData>().GetInstance().GetPLStatus(charaID).GetHP();
		}
		int nextCharaHP = Singleton<StatusData>().GetInstance().GetPLStatus(bCharacters[aliveObjID]->GetCharaID()).GetHP();

		// ���̍ŏ��l�Ɣ�ׂāA�ύX�����邩�`�F�b�N
		int changeIndex = -1;
		int subMin = 0;
		for (int i = 0; i < num; ++i)
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
