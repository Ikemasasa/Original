#include "TurnManager.h"

#include <random>
#include <numeric>

#include "lib/Random.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CommandBase.h"
#include "DropData.h"
#include "EffectManager.h"
#include "ProductionAttack.h"
#include "ProductionEscape.h"
#include "ProductionGuard.h"
#include "ProductionSkill.h"
#include "ProductionUseItem.h"
#include "Singleton.h"

void TurnManager::Initialize(const std::vector<std::shared_ptr<BattleCharacter>>& battleCharaArray)
{
	SortOrder(battleCharaArray);

	// �G�t�F�N�g�ǂݍ���
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/death.efk"		, DEATH_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/heal_potion.efk"  , HEAL_POTION_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/magic_potion.efk" , MAGIC_POTION_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/explosion.efk"    , ITEM_DAMAGE_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/buff.efk"		    , BUFF_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/debuff.efk"		, DEBUFF_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/damage.efk"		, DAMAGE_EFFECT_SLOT);

	mCharacterHealth.Initialize(Vector2(HEALTH_BOARD_X, HEALTH_BOARD_Y));

	mIsBeginnig = true;
}

void TurnManager::Update(const BattleCharacterManager* bcm)
{
	// �w���X
	std::vector<Status> statusArray;
	int activeNo = -1;
	for (int i = 0; i < bcm->GetBCharacters().size(); ++i)
	{
		BattleCharacter* bc = bcm->GetChara(i);
		if (bc->GetType() != Character::PLAYER) break;

		// BCharacters�͍ŏ��Ƀv���C���[���͂����Ă邩�� 0~�l�����őS���ɃA�N�Z�X�ł���

		if (bc == mOrder.front()) activeNo = i;
		statusArray.push_back(*bc->GetStatus());
	}
	mCharacterHealth.Update(statusArray, activeNo);

	// ���U���g�Ȃ�return
	if (BattleState::GetInstance().CheckState(BattleState::State::RESULT))
	{
		mIsTurnFinished = false;
		return;
	}

	// �R�}���h�I��
	if (!mProduction)
	{
		mIsTurnFinished = false;
		GetMoveChara()->SetMotion(Character::IDLE);

		// BattleCharaManager��update�ŃR�}���h�����܂�����
		if (GetMoveChara()->GetCommand()->IsBehaviourEnable())
		{
			// ���o�J�n
			BeginProduction();
		}
	}
	else // ���o��
	{
		mProduction->Update(bcm);
		mProduction->UpdateDeathMotion();

		// ���o���I��������
		if (mProduction->IsFinished())
		{
			// behaviour �� none�ɂ���
			GetMoveChara()->GetCommand()->BehaviourFinished();

			// �^�[���I�����o�t�̃^�[���������炷
			mOrder.front()->GetStatus()->AdvanceBuffTurn();

			// mOrder��i�߂�
			AdvanceOrder(bcm->GetBCharacters());

			// �K�[�h���Ă�ꍇ�͉�������
			GetMoveChara()->GetStatus()->SetGuardFlag(false);

			// ���o���폜
			mProduction.reset();

			// �^�[���I���t���O�𗧂Ă�
			mIsTurnFinished = true;
		}
	}

}

void TurnManager::Render()
{
	if (mProduction)
	{
		mProduction->Render(); // �U���̃_���[�W(amount)�Ƃ���\��
	}

	mCharacterHealth.Render();
}

void TurnManager::SortOrder(const std::vector<std::shared_ptr<BattleCharacter>>& battleCharaArray)
{
	// min����max�܂ł̒l�������_���ɕ��ׂ��z������yatu
	auto RandArrayNoDuplicate = [](const int min, const int max)
	{
		const int DIST = max - min + 1;

		// �����Z���k�c�C�X�^�ŗ������쐬����
		std::random_device rnd;
		std::mt19937 mt(rnd());

		// min~max�̒l�𐶐����āA�V���b�t������
		std::vector<int> ret(DIST);
		std::iota(ret.begin(), ret.end(), min);
		std::shuffle(ret.begin(), ret.end(), mt);

		return ret;
	};

	std::map<int, std::vector<BattleCharacter*>> spdOrder;

	// �}�b�v�̃L�[�͏����Ƀ\�[�g����Ă�
	const int WIDTH = 5; // �X�s�[�h�̗����̕�+-
	for (auto& chara : battleCharaArray)
	{
		int spd = chara->GetStatus()->GetSpd();
		spd = Math::Max(0, Random::RandomRange(spd - WIDTH, spd + WIDTH));
		spdOrder[spd].push_back(chara.get());
	}

	// �~���ɑ�����������烊�o�[�X�C�e���[�^
	for (auto it = spdOrder.rbegin(); it != spdOrder.rend(); ++it)
	{
		// 2�ȏ�Ȃ炻�̒����烉���_���Ō��߂�
		if (it->second.size() >= 2)
		{
			std::vector<int> randArr = RandArrayNoDuplicate(0, it->second.size() - 1);
			for (size_t i = 0; i < it->second.size(); ++i)
			{
				mOrder.push(it->second.at(randArr[i]));
			}
		}
		else
		{
			mOrder.push(it->second.back());
		}
	}
}

void TurnManager::BeginProduction()
{
	CommandBase::Behaviour behaviour = GetMoveChara()->GetCommand()->GetBehaviour();
	switch (behaviour)
	{
	case CommandBase::Behaviour::USE_ITEM: mProduction = std::make_unique<ProductionUseItem>(); break;
	case CommandBase::Behaviour::GUARD:    mProduction = std::make_unique<ProductionGuard>();   break;
	case CommandBase::Behaviour::ATTACK:   mProduction = std::make_unique<ProductionAttack>();  break;
	case CommandBase::Behaviour::SKILL:    mProduction = std::make_unique<ProductionSkill>();  break;
	case CommandBase::Behaviour::ESCAPE:   mProduction = std::make_unique<ProductionEscape>();  break;
	}

	mProduction->Initialize();

	int moveCharaID = GetMoveChara()->GetObjID();
	std::vector<int> targetCharaIDs = GetMoveChara()->GetCommand()->GetTargetObjIDs();
	mProduction->Begin(moveCharaID, targetCharaIDs);
}

void TurnManager::AdvanceOrder(const std::vector<std::shared_ptr<BattleCharacter>>& battleCharaArray)
{
	mOrder.pop();

	// mOrder�𐮗�����
	while (true)
	{
		if (mOrder.empty())
		{
			SortOrder(battleCharaArray);
		}
		

		// ���Ԃ������L�������|����Ă�����
		if (mOrder.front()->GetStatus()->IsDead())
		{
			mOrder.pop();
		}
		else break;
	}
}
