#include "TurnManager.h"

#include <random>
#include <numeric>

#include "BattleActorManager.h"
#include "CommandBase.h"
#include "EffectManager.h"
#include "ProductionAttack.h"
#include "ProductionUseItem.h"
#include "Singleton.h"

void TurnManager::Initialize(const std::vector<std::shared_ptr<BattleActor>>& battleActorArray)
{
	SortOrder(battleActorArray);

	// �G�t�F�N�g�ǂݍ���
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/Death/Death.efk", DEATH_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/HealPotion/heal_potion.efk", HEAL_POTION_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/HealPotion/magic_potion.efk", MAGIC_POTION_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/Explosion/explosion.efk", ITEM_DAMAGE_EFFECT_SLOT);
}

void TurnManager::Update(const BattleActorManager* bam)
{
	// �R�}���h�I��
	if (!mProduction)
	{
		mIsTurnFinished = false;

		// BattleActorManager��update�ŃR�}���h�����܂�����
		if (GetMoveActor()->GetCommand()->IsBehaviourEnable())
		{
			// ���o�J�n
			BeginProduction();
		}
	}
	else // ���o��
	{
		mProduction->Update(bam);

		// ���o���I��������
		if (mProduction->IsFinished())
		{
			// behaviour �� none�ɂ���
			GetMoveActor()->GetCommand()->BehaviourFinished();

			// mOrder�𐮗�����
			mOrder.pop(); // �����moveactor�͍폜
			OrganizeOrder(bam->GetBActors());

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
}

void TurnManager::ToResult()
{
	mIsResult = true;
}

void TurnManager::SortOrder(const std::vector<std::shared_ptr<BattleActor>>& battleActorArray)
{
	// min����max�܂ł̒l�������_���ɕ��ׂ��z������yatu
	auto RandArrayNoDuplicate = [](const int min, const int max)
	{
		const int DIST = max - min + 1;

		// �����Z���k�c�C�X�^�ŗ������쐬����
		std::random_device rnd;
		std::mt19937 mt(rnd());

		// 0~DIST�̒l�𐶐����āA�V���b�t������
		std::vector<int> ret(DIST);
		std::iota(ret.begin(), ret.end(), min);
		std::shuffle(ret.begin(), ret.end(), mt);

		return ret;
	};

	std::map<int, std::vector<BattleActor*>> agiOrder;

	// �}�b�v�̃L�[�͏����Ƀ\�[�g����Ă�
	for (auto& actor : battleActorArray)
	{
		agiOrder[actor->GetStatus()->agi].push_back(actor.get());
	}

	// �~���ɑ�����������烊�o�[�X�C�e���[�^
	for (auto it = agiOrder.rbegin(); it != agiOrder.rend(); ++it)
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
	CommandBase::Behaviour behaviour = GetMoveActor()->GetCommand()->GetBehaviour();
	switch (behaviour)
	{
	case CommandBase::Behaviour::ATTACK:   mProduction = std::make_unique<ProductionAttack>(); break;
	case CommandBase::Behaviour::USE_ITEM: mProduction = std::make_unique<ProductionUseItem>(); break;
	}

	mProduction->Initialize();

	int moveActorID = GetMoveActor()->GetObjID();
	int targetActorID = GetMoveActor()->GetCommand()->GetTargetObjID();
	mProduction->Begin(moveActorID, targetActorID);
}

void TurnManager::OrganizeOrder(const std::vector<std::shared_ptr<BattleActor>>& battleActorArray)
{
	// mOrder�𐮗�����
	while (true)
	{
		// mOrder����Ȃ�܂����Ԃ����
		if (mOrder.empty())
		{
			SortOrder(battleActorArray);
		}

		// ���Ԃ������A�N�^���|����Ă�����
		if (mOrder.front()->GetStatus()->IsDead())
		{
			mOrder.pop();
		}
		else break;
	}
}
