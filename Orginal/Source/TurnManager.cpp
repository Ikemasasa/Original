#include "TurnManager.h"

#include <random>
#include <numeric>

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "EffectManager.h"
#include "ProductionAttack.h"
#include "ProductionUseItem.h"
#include "Singleton.h"

void TurnManager::Initialize(const std::vector<std::shared_ptr<BattleCharacter>>& battleCharaArray)
{
	SortOrder(battleCharaArray);

	// エフェクト読み込み
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/Death/Death.efk", DEATH_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/HealPotion/heal_potion.efk", HEAL_POTION_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/HealPotion/magic_potion.efk", MAGIC_POTION_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/Explosion/explosion.efk", ITEM_DAMAGE_EFFECT_SLOT);
}

void TurnManager::Update(const BattleCharacterManager* bcm)
{
	// コマンド選択中
	if (!mProduction)
	{
		mIsTurnFinished = false;

		// BattleCharaManagerのupdateでコマンドが決まったら
		if (GetMoveChara()->GetCommand()->IsBehaviourEnable())
		{
			// 演出開始
			BeginProduction();
		}
	}
	else // 演出中
	{
		mProduction->Update(bcm);

		// 演出が終了したら
		if (mProduction->IsFinished())
		{
			// behaviour を noneにする
			GetMoveChara()->GetCommand()->BehaviourFinished();

			// mOrderを整理する
			mOrder.pop(); // 今回のmoveCharaは削除
			OrganizeOrder(bcm->GetBCharacters());

			// 演出情報削除
			mProduction.reset();

			// ターン終了フラグを立てる
			mIsTurnFinished = true;
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

void TurnManager::ToResult()
{
	mIsResult = true;
}

void TurnManager::SortOrder(const std::vector<std::shared_ptr<BattleCharacter>>& battleCharaArray)
{
	// minからmaxまでの値をランダムに並べた配列を作るyatu
	auto RandArrayNoDuplicate = [](const int min, const int max)
	{
		const int DIST = max - min + 1;

		// メルセンヌツイスタで乱数を作成する
		std::random_device rnd;
		std::mt19937 mt(rnd());

		// 0~DISTの値を生成して、シャッフルする
		std::vector<int> ret(DIST);
		std::iota(ret.begin(), ret.end(), min);
		std::shuffle(ret.begin(), ret.end(), mt);

		return ret;
	};

	std::map<int, std::vector<BattleCharacter*>> agiOrder;

	// マップのキーは昇順にソートされてる
	for (auto& chara : battleCharaArray)
	{
		agiOrder[chara->GetStatus()->agi].push_back(chara.get());
	}

	// 降順に代入したいからリバースイテレータ
	for (auto it = agiOrder.rbegin(); it != agiOrder.rend(); ++it)
	{
		// 2個以上ならその中からランダムで決める
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
	case CommandBase::Behaviour::ATTACK:   mProduction = std::make_unique<ProductionAttack>(); break;
	case CommandBase::Behaviour::USE_ITEM: mProduction = std::make_unique<ProductionUseItem>(); break;
	}

	mProduction->Initialize();

	int moveCharaID = GetMoveChara()->GetObjID();
	int targetCharaID = GetMoveChara()->GetCommand()->GetTargetObjID();
	mProduction->Begin(moveCharaID, targetCharaID);
}

void TurnManager::OrganizeOrder(const std::vector<std::shared_ptr<BattleCharacter>>& battleCharaArray)
{
	// mOrderを整理する
	while (true)
	{
		// mOrderが空ならまた順番を作る
		if (mOrder.empty())
		{
			SortOrder(battleCharaArray);
		}

		// 順番が来たアクタが倒されていたら
		if (mOrder.front()->GetStatus()->IsDead())
		{
			mOrder.pop();
		}
		else break;
	}
}
