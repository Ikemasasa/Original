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

	// TODO: ���݃o�t�A�C�e���͖�����
	void CalcAmountValue(const ItemData::ItemParam* param);  // �Œ�l�v�Z 
	void CalcAmountPercent(const ItemData::ItemParam* param);// �����l�v�Z

public:
	ProductionUseItem() = default;
	~ProductionUseItem() = default;

	void Initialize();
	void Update(const BattleCharacterManager* bcm);
	void Render();
};