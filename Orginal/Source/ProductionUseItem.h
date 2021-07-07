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

	// TODO: ���݃o�t�A�C�e���͖�����
	void CalcAmountValue();  // �Œ�l�v�Z 
	void CalcAmountPercent();// �����l�v�Z

public:
	void Initialize();
	void Update(const BattleActorManager* bam);
	void Render();
};