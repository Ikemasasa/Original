#pragma once
#include "IBattleProduction.h"

#include "FontValue.h"
#include "ItemData.h"

class BattleCharacterManager;

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
	std::vector<int> mEffectInstHandles;
	Vector3 mFontRGB;

private:
	void StateInit();
	void StateUseItemWait();
	void StateWaitEffect();
	void StateWait();

	// TODO: 現在バフアイテムは未実装
	void CalcAmountValue(const ItemData::ItemParam* param);  // 固定値計算 
	void CalcAmountPercent(const ItemData::ItemParam* param);// 割合値計算

public:
	ProductionUseItem() = default;
	~ProductionUseItem() = default;

	void Initialize();
	void Update(const BattleCharacterManager* bcm);
	void Render();
};