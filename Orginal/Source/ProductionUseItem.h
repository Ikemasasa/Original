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
	ItemData::ItemParam mUseItemParam;

private:
	void StateInit();
	void StateUseItemWait();
	void StateWaitEffect();
	void StateWait();

	// TODO: 現在バフアイテムは未実装
	void CalcAmountValue();  // 固定値計算 
	void CalcAmountPercent();// 割合値計算

public:
	void Initialize();
	void Update(const BattleActorManager* bam);
	void Render();
};