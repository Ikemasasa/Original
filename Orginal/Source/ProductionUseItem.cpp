#include "ProductionUseItem.h"

#include "BattleCharacterManager.h"
#include "CameraManager.h"
#include "CommandBase.h"
#include "DamageCalculator.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "Item.h"
#include "Singleton.h"
#include "Sound.h"
#include "TurnManager.h"

void ProductionUseItem::Initialize()
{
	mProductionValue.Initialize();
}

void ProductionUseItem::Update()
{
	switch (mState)
	{
	case State::INIT: // 初期設定

		StateInit();
		// break;

	case State::USE_ITEM_WAIT: // アイテム使用モーション待機
		StateUseItemWait();
		break;

	case State::WAIT_EFFECT: // エフェクト再生終了待機
		StateWaitEffect();
		break;

	case State::WAIT: // 待機時間
		StateWait();

		break;

	}
}

void ProductionUseItem::Render()
{
	mProductionValue.Render();
}

void ProductionUseItem::StateInit()
{
	// アイテム使用モーションセット
	mMoveChara->SetMotionOnce(Character::USE_ITEM, Character::IDLE);

	// 使用アイテム取得
	const UseItemData::Param* param = mMoveChara->GetCommand()->GetItemParam();

	// 効果量計算(mAmountに代入される) 
	switch (param->rate)
	{
	case UseItemData::VALUE:   CalcAmountValue(param); break;
	case UseItemData::PERCENT: CalcAmountPercent(param); break;
	}

	// ダメージ計算
	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		switch (param->base->type)
		{
		case ItemData::DAMAGE:
			mHPAmounts[i] = DamageCalculator::CalcItemDamage(mHPAmounts[i], mTargetCharas[i]->GetStatus());

			mTargetCharas[i]->GetStatus()->SubHP(mHPAmounts[i]);
			mTargetCharas[i]->GetStatus()->SubMP(mMPAmounts[i]);
			break;

		case ItemData::HEAL:
			mTargetCharas[i]->GetStatus()->AddHP(mHPAmounts[i]);
			mTargetCharas[i]->GetStatus()->AddMP(mMPAmounts[i]);
			break;
		}
	}

	// アイテムを減らす
	mMoveChara->GetInventory()->Sub(param->base->id);
	++mState;
}

void ProductionUseItem::StateUseItemWait()
{
	// モーションが終わったら
	if (mMoveChara->IsMotionFinished())
	{
		// アイテム情報取得
		const UseItemData::Param* param = mMoveChara->GetCommand()->GetItemParam();

		// サウンド、エフェクト取得
		Sound::Kind sound = (Sound::Kind)param->soundSlot;
		EffectData::Kind effect = (EffectData::Kind)param->effectSlot;

		// ターゲットキャラ全捜査
		for (const auto& target : mTargetCharas)
		{
			if (param->base->type == ItemData::DAMAGE)
			{
				// 1フレームで当てるため、当たり判定とかはいらないけど、
				// HitProductionに合わせるためにSPHEREを作る
				Vector3 center = target->GetTargetPos();
				float radius = target->GetCapsule().radius;
				SPHERE sphere = { center, radius };

				IBattleProduction::HitProduction(sphere, effect, sound);
			}

			if (param->base->type == ItemData::HEAL)
			{
				IBattleProduction::HealProduction(effect, sound);
			}

		}

		++mState;
	}
}

void ProductionUseItem::StateWaitEffect()
{
	// エフェクトが再生中かチェック
	bool isPlay = false;
	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		if (Singleton<EffectManager>().GetInstance().IsPlay(mEffectInstHandles[i]))
		{
			isPlay = true;
			break;
		}
	}

	// 全て終了すれば次のステート
	if (!isPlay)
	{
		++mState;
	}
}

void ProductionUseItem::CalcAmountValue(const UseItemData::Param* param)
{
	for (const auto& target : mTargetCharas)
	{
		mHPAmounts.push_back(param->hpValue);
		mMPAmounts.push_back(param->mpValue);
	}
}

void ProductionUseItem::CalcAmountPercent(const UseItemData::Param* param)
{
	// hpValueは%なので、100で割る
	const float MAX_PERCENT = 100.0f;
	int amountHP = (int)(mMoveChara->GetStatus()->GetMaxHP() * (param->hpValue / MAX_PERCENT));
	int amountMP = (int)(mMoveChara->GetStatus()->GetMaxMP() * (param->mpValue / MAX_PERCENT));

	mHPAmounts.push_back(amountHP);
	mMPAmounts.push_back(amountMP);
}

