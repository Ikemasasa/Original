#include "TurnManager.h"

#include <random>

#include "BattleActorManager.h"
#include "EffectManager.h"
#include "ProductionAttack.h"
#include "Singleton.h"

void TurnManager::Initialize(const std::vector<std::shared_ptr<BattleActor>>& battleActorArray)
{
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/Death/Death.efk", DEATH_EFFECT_SLOT);
}

void TurnManager::Update(const BattleActorManager* bam)
{
	// �R�}���h�I��
	if (!mProduction)
	{
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
		if (mProduction->GetIsFinished())
		{
			// behaviour �� none�ɂ���
			GetMoveActor()->GetCommand()->BehaviourFinished();

			// mOrder�𐮗�����
			mOrder.pop(); // �����moveactor�͍폜
			OrganizeOrder(bam);
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

void TurnManager::SortOrder(const std::vector<std::shared_ptr<BattleActor>>& battleActorArray)
{
	// min����max�܂ł̒l�������_���ɂ����z������yatu
	auto RandArrayNoDuplicate = [](const int min, const int max)
	{
		const int dist = max - min + 1;

		std::vector<int> temp;
		temp.reserve(dist);
		for (int i = min; i <= max; ++i) temp.push_back(i);

		std::random_device seedGen;
		std::default_random_engine engine(seedGen());

		for (int i = dist - 1; i > 0; --i)
		{
			int target = std::uniform_int_distribution<int>(i, dist - 1)(engine);
			if (i != target) std::swap(temp[i], temp[target]);
		}

		return temp;
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
		if (it->second.size() > 2)
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
	case CommandBase::Behaviour::ATTACK: mProduction = std::make_unique<ProductionAttack>(); break;
	}

	// �����������Ă�����(�G���[�΍�)
	if (mProduction)
	{
		mProduction->Initialize();
		
		int moveActorID = GetMoveActor()->GetObjID();
		int targetActorID = GetMoveActor()->GetCommand()->GetTargetObjID();
		mProduction->Begin(moveActorID, targetActorID);
	}
}

void TurnManager::OrganizeOrder(const BattleActorManager* bam)
{
	// mOrder�𐮗�����

	while (true)
	{
		// mOrder����Ȃ�܂����Ԃ����
		if (mOrder.empty())
		{
			SortOrder(bam->GetBActors());
		}

		// ���Ԃ������A�N�^���|����Ă�����
		if (!mOrder.front()->GetExist())
		{
			mOrder.pop();
		}
		else break;
	}
}
