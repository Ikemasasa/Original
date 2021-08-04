#include "ProductionUseItem.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
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
		mTargetChara = bcm->GetChara(mTargetCharaID);

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

	// ダメージアイテムのhpvalueはマイナスになってるからヒールで回復、ダメージ両方できる
	mTargetChara->GetStatus()->HealHP(mHPAmount);
	mTargetChara->GetStatus()->HealMP(mMPAmount);
	mMoveChara->GetInventory()->Sub(mMoveChara->GetCommand()->GetItemIndex());

	// エフェクト決定
	if (mHPAmount < 0) mEffectSlot = TurnManager::ITEM_DAMAGE_EFFECT_SLOT;
	else if (mMPAmount > 0) mEffectSlot = TurnManager::MAGIC_POTION_EFFECT_SLOT;
	else if (mHPAmount > 0) mEffectSlot = TurnManager::HEAL_POTION_EFFECT_SLOT;


	++mState;
}

void ProductionUseItem::StateUseItemWait()
{
	// モーションが終わったら
	if (mMoveChara->IsMotionFinished())
	{
		// エフェクト再生
		Vector3 effectPos = mTargetChara->GetPos();
		mEffectInstHandle = Singleton<EffectManager>().GetInstance().Play(mEffectSlot, effectPos);
		++mState;
	}
}

void ProductionUseItem::StateWaitEffect()
{
	bool isPlay = Singleton<EffectManager>().GetInstance().IsPlay(mEffectInstHandle);

	// エフェクトの再生がおわったら 
	if (!isPlay)
	{
		// 死んでたら死亡エフェクト
		if (mTargetChara->GetStatus()->IsDead())
		{
			// Existをfalseにして、エフェクトを再生する
			mTargetChara->SetExist(false); // 見えなくする
			Vector3 effectPos(mTargetChara->GetPos().x, mTargetChara->GetPos().y + mTargetChara->GetLocalAABB().max.y * 0.5f, mTargetChara->GetPos().z);
			Singleton<EffectManager>().GetInstance().Play(TurnManager::DEATH_EFFECT_SLOT, effectPos);// えっふぇくと
		}

		// HPの文字セット
		int addNum = 0;
		const float ADDJUST_Y = 0.1f;
		if (mHPAmount > 0)
		{
			Vector3 pos(mTargetChara->GetPos().x, mTargetChara->GetAABB().max.y - ADDJUST_Y * addNum, mTargetChara->GetPos().z);
			mProductionValue.Add(mHPAmount, pos, HEAL_HP_RGB);
			++addNum;
		}
		else if (mHPAmount < 0)
		{
			Vector3 pos(mTargetChara->GetPos().x, mTargetChara->GetAABB().max.y - ADDJUST_Y * addNum, mTargetChara->GetPos().z);
			mProductionValue.Add(mHPAmount, pos, DAMAGE_RGB);
			++addNum;
		}

		// MPの文字セット
		if (mMPAmount > 0)
		{
			Vector3 pos(mTargetChara->GetPos().x, mTargetChara->GetAABB().max.y - ADDJUST_Y * addNum, mTargetChara->GetPos().z);
			mProductionValue.Add(mHPAmount, pos, DAMAGE_RGB);
			++addNum;
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
	mHPAmount = (int)(mMoveChara->GetStatus()->maxHP * (param->hpValue / MAX_PERCENT));
	mMPAmount = (int)(mMoveChara->GetStatus()->maxMP * (param->mpValue / MAX_PERCENT));
}

