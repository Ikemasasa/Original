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
	// コマンド選択中
	if (!mProduction)
	{
		// BattleActorManagerのupdateでコマンドが決まったら
		if (GetMoveActor()->GetCommand()->IsBehaviourEnable())
		{
			// 演出開始
			BeginProduction();
		}
	}
	else // 演出中
	{
		mProduction->Update(bam);

		// 演出が終了したら
		if (mProduction->GetIsFinished())
		{
			// behaviour を noneにする
			GetMoveActor()->GetCommand()->BehaviourFinished();

			// mOrderを整理する
			mOrder.pop(); // 今回のmoveactorは削除
			OrganizeOrder(bam);
		}
	}

}

void TurnManager::Render()
{
	if (mProduction)
	{
		mProduction->Render(); // 攻撃のダメージ(amount)とかを表示
	}
}

void TurnManager::SortOrder(const std::vector<std::shared_ptr<BattleActor>>& battleActorArray)
{
	// minからmaxまでの値をランダムにした配列を作るyatu
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

	// マップのキーは昇順にソートされてる
	for (auto& actor : battleActorArray)
	{
		agiOrder[actor->GetStatus()->agi].push_back(actor.get());
	}

	// 降順に代入したいからリバースイテレータ
	for (auto it = agiOrder.rbegin(); it != agiOrder.rend(); ++it)
	{
		// 2個以上ならその中からランダムで決める
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

	// しっかり作られていたら(エラー対策)
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
	// mOrderを整理する

	while (true)
	{
		// mOrderが空ならまた順番を作る
		if (mOrder.empty())
		{
			SortOrder(bam->GetBActors());
		}

		// 順番が来たアクタが倒されていたら
		if (!mOrder.front()->GetExist())
		{
			mOrder.pop();
		}
		else break;
	}
}
