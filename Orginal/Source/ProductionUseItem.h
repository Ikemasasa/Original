#pragma once
#include "IBattleProduction.h"

#include "FontValue.h"
#include "ItemData.h"

class ProductionUseItem : public IBattleProduction
{
	enum State
	{
		INIT,
		USE_ITEM_WAIT,
		WAIT_EFFECT,
		WAIT,

		END
	};

	int mState = 0;
	int mHPAmount;
	int mMPAmount;
	int mEffectSlot;
	int mEffectInstHandle;

private:
	void StateInit();
	void StateUseItemWait();
	void StateWaitEffect();
	void StateWait();

	// TODO: 現在バフアイテムは未実装
	void CalcAmountValue(const ItemData::ItemParam* param);  // 固定値計算 
	void CalcAmountPercent(const ItemData::ItemParam* param);// 割合値計算

public:
	void Initialize();
	void Update(const BattleActorManager* bam);
	void Render();
};