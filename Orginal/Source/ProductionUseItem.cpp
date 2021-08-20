#include "ProductionUseItem.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "DamageCalculator.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "Item.h"
#include "Singleton.h"


void ProductionUseItem::Initialize()
{
	mProductionValue.Initialize();
}

void ProductionUseItem::Update(const BattleCharacterManager* bcm)
{
	switch (mState)
	{
	case State::INIT: // 初期設定

		// IDからポインタ取得
		mMoveChara = bcm->GetChara(mMoveCharaID);
		for (int i = 0; i < mTargetCharaIDs.size(); ++i)
		{
			mTargetCharas.push_back(bcm->GetChara(mTargetCharaIDs[i]));
		}

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

	mProductionValue.Update();
}

void ProductionUseItem::Render()
{
	mProductionValue.Render();
}

void ProductionUseItem::StateInit()
{
	// アイテム使用モーションセット
	mMoveChara->SetMotion(SkinnedMesh::USE_ITEM, false);

	// 使用アイテム取得
	const ItemData::ItemParam* param = mMoveChara->GetInventory()->GetItemParam(mMoveChara->GetCommand()->GetItemIndex());

	// 効果量計算(m~Amountに代入される) 
	switch (param->rate)
	{
	case ItemData::VALUE:  CalcAmountValue(param); break;
	case ItemData::PERCENT: CalcAmountPercent(param); break;
	}

	for (int i = 0; i < mTargetCharas.size(); ++i)
	{
		switch (param->effect)
		{
		case ItemData::Effect::DAMAGE:
			mHPAmount = DamageCalculator::CalcItemDamage(mHPAmount, mTargetCharas[i]->GetStatus());

			mTargetCharas[i]->GetStatus()->SubHP(mHPAmount);
			mTargetCharas[i]->GetStatus()->SubMP(mMPAmount);
			break;

		case ItemData::Effect::HEAL:
			mTargetCharas[i]->GetStatus()->AddHP(mHPAmount);
			mTargetCharas[i]->GetStatus()->AddMP(mMPAmount);
			break;
		}
	}


	// エフェクト決定
	switch (param->effect)
	{
	case ItemData::Effect::DAMAGE:
		mEffectSlot = TurnManager::ITEM_DAMAGE_EFFECT_SLOT; 
		mFontRGB = DAMAGE_RGB;
		break;
	case ItemData::Effect::HEAL: 
		if (mHPAmount > 0)
		{
			mEffectSlot = TurnManager::HEAL_POTION_EFFECT_SLOT;
			mFontRGB = HEAL_HP_RGB;
		}
		else if (mMPAmount > 0)
		{
			mEffectSlot = TurnManager::MAGIC_POTION_EFFECT_SLOT;
			mFontRGB = HEAL_MP_RGB;
		}
		break;
	}

	// アイテムを減らす
	mMoveChara->GetInventory()->Sub(mMoveChara->GetCommand()->GetItemIndex());

	++mState;
}

void ProductionUseItem::StateUseItemWait()
{
	// モーションが終わったら
	if (mMoveChara->IsMotionFinished())
	{
		// エフェクト再生
		for (int i = 0; i < mTargetCharas.size(); ++i)
		{
			Vector3 effectPos = mTargetCharas[i]->GetPos();
			mEffectInstHandles.push_back(Singleton<EffectManager>().GetInstance().Play(mEffectSlot, effectPos));
			++mState;
		}
	}
}

void ProductionUseItem::StateWaitEffect()
{
	std::vector<bool> isPlay;
	isPlay.resize(mTargetCharas.size());
	for (int i = 0; i < mTargetCharas.size(); ++i)
	{
		isPlay[i] = Singleton<EffectManager>().GetInstance().IsPlay(mEffectInstHandles[i]);
	}

	// 終了していないエフェクトがあれば処理しない
	if(std::find(isPlay.begin(), isPlay.end(), false) == isPlay.end()) return; 

	for (int i = 0; i < mTargetCharas.size(); ++i)
	{
		// 死んでたら死亡エフェクト
		if (mTargetCharas[i]->GetStatus()->IsDead())
		{
			// Existをfalseにして、エフェクトを再生する
			mTargetCharas[i]->SetExist(false); // 見えなくする
			Vector3 effectPos(mTargetCharas[i]->GetPos().x, mTargetCharas[i]->GetPos().y + mTargetCharas[i]->GetLocalAABB().max.y * 0.5f, mTargetCharas[i]->GetPos().z);
			Singleton<EffectManager>().GetInstance().Play(TurnManager::DEATH_EFFECT_SLOT, effectPos);// えっふぇくと
		}

		// ダメージ、回復の文字セット
		const float ADDJUST_Y = 0.1f;
		const int AMOUNT_NUM = 2;
		const int amounts[AMOUNT_NUM] = { mHPAmount, mMPAmount };
		for (int k = 0; k < AMOUNT_NUM; ++k)
		{
			Vector3 pos(mTargetCharas[i]->GetPos().x, mTargetCharas[i]->GetAABB().max.y - ADDJUST_Y * k, mTargetCharas[i]->GetPos().z);
			if(amounts[k] > 0) mProductionValue.Add(amounts[k], pos, mFontRGB);
		}

		++mState;
	}


}

void ProductionUseItem::StateWait()
{
	static float waitTimer = 0.0f;
	waitTimer += GameManager::elapsedTime;

	const float WAIT_SEC = 1.5f;
	if (waitTimer >= WAIT_SEC)
	{
		mState = 0;
		waitTimer = 0.0f;
		mIsFinished = true;
	}
}

void ProductionUseItem::CalcAmountValue(const ItemData::ItemParam* param)
{
	mHPAmount = param->hpValue;
	mMPAmount = param->mpValue;
}

void ProductionUseItem::CalcAmountPercent(const ItemData::ItemParam* param)
{
	// hpValueは%なので、100で割る
	const float MAX_PERCENT = 100.0f;
	mHPAmount = (int)(mMoveChara->GetStatus()->GetMaxHP() * (param->hpValue / MAX_PERCENT));
	mMPAmount = (int)(mMoveChara->GetStatus()->GetMaxMP() * (param->mpValue / MAX_PERCENT));
}

