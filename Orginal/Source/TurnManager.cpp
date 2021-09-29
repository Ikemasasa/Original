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

	// エフェクト読み込み
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/death.efk"		, DEATH_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/heal_potion.efk"  , HEAL_POTION_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/magic_potion.efk" , MAGIC_POTION_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/explosion.efk"    , ITEM_DAMAGE_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/buff.efk"		    , BUFF_EFFECT_SLOT);
	Singleton<EffectManager>().GetInstance().Create(u"Data/Effect/debuff.efk"		, DEBUFF_EFFECT_SLOT);

	mIsBeginnig = true;
}

void TurnManager::Update(const BattleCharacterManager* bcm)
{
	// リザルトならreturn
	if (BattleState::GetInstance().CheckState(BattleState::State::RESULT))
	{
		mIsTurnFinished = false;
		return;
	}

	// コマンド選択中
	if (!mProduction)
	{
		mIsTurnFinished = false;
		GetMoveChara()->SetMotion(Character::IDLE);

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

			// ターン終了時バフのターン数を減らす
			mOrder.front()->GetStatus()->AdvanceBuffTurn();

			// mOrderを進める
			AdvanceOrder(bcm->GetBCharacters());

			// ガードしてる場合は解除する
			GetMoveChara()->GetStatus()->SetGuardFlag(false);

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

void TurnManager::SortOrder(const std::vector<std::shared_ptr<BattleCharacter>>& battleCharaArray)
{
	// minからmaxまでの値をランダムに並べた配列を作るyatu
	auto RandArrayNoDuplicate = [](const int min, const int max)
	{
		const int DIST = max - min + 1;

		// メルセンヌツイスタで乱数を作成する
		std::random_device rnd;
		std::mt19937 mt(rnd());

		// min~maxの値を生成して、シャッフルする
		std::vector<int> ret(DIST);
		std::iota(ret.begin(), ret.end(), min);
		std::shuffle(ret.begin(), ret.end(), mt);

		return ret;
	};

	std::map<int, std::vector<BattleCharacter*>> spdOrder;

	// マップのキーは昇順にソートされてる
	const int WIDTH = 5; // スピードの乱数の幅+-
	for (auto& chara : battleCharaArray)
	{
		int spd = chara->GetStatus()->GetSpd();
		spd = Math::Max(0, Random::RandomRange(spd - WIDTH, spd + WIDTH));
		spdOrder[spd].push_back(chara.get());
	}

	// 降順に代入したいからリバースイテレータ
	for (auto it = spdOrder.rbegin(); it != spdOrder.rend(); ++it)
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

	// mOrderを整理する
	while (true)
	{
		if (mOrder.empty())
		{
			SortOrder(battleCharaArray);
		}
		

		// 順番が来たキャラが倒されていたら
		if (mOrder.front()->GetStatus()->IsDead())
		{
			mOrder.pop();
		}
		else break;
	}
}
