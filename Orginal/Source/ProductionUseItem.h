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

	// TODO: ���݃o�t�A�C�e���͖�����
	void CalcAmountValue(const ItemData::ItemParam* param);  // �Œ�l�v�Z 
	void CalcAmountPercent(const ItemData::ItemParam* param);// �����l�v�Z

public:
	void Initialize();
	void Update(const BattleActorManager* bam);
	void Render();
};