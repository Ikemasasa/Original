#pragma once
#include "IBattleProduction.h"

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
	Vector3 mFontRGB;

private:
	void StateInit();
	void StateUseItemWait();
	void StateWaitEffect();

	void CalcAmountValue(const UseItemData::Param* param);  // 固定値計算 
	void CalcAmountPercent(const UseItemData::Param* param);// 割合値計算

public:
	ProductionUseItem() = default;
	~ProductionUseItem() = default;

	void Initialize();
	void Update();
	void Render();
};