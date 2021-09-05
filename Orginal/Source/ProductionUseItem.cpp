#include "ProductionUseItem.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "DamageCalculator.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "Item.h"
#include "Singleton.h"
#include "TurnManager.h"

void ProductionUseItem::Initialize()
{
	mProductionValue.Initialize();
}

void ProductionUseItem::Update(const BattleCharacterManager* bcm)
{
	switch (mState)
	{
	case State::INIT: // �����ݒ�

		// ID����|�C���^�擾
		mMoveChara = bcm->GetChara(mMoveCharaID);
		for (size_t i = 0; i < mTargetCharaIDs.size(); ++i)
		{
			mTargetCharas.push_back(bcm->GetChara(mTargetCharaIDs[i]));
		}

		StateInit();
		// break;

	case State::USE_ITEM_WAIT: // �A�C�e���g�p���[�V�����ҋ@
		StateUseItemWait();
		break;

	case State::WAIT_EFFECT: // �G�t�F�N�g�Đ��I���ҋ@
		StateWaitEffect();
		break;

	case State::WAIT: // �ҋ@����
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
	// �A�C�e���g�p���[�V�����Z�b�g
	mMoveChara->SetMotion(SkinnedMesh::USE_ITEM, false);

	// �g�p�A�C�e���擾
	const UseItemData::Param* param = mMoveChara->GetCommand()->GetItemParam();

	// ���ʗʌv�Z(m~Amount�ɑ�������) 
	switch (param->rate)
	{
	case UseItemData::VALUE:   CalcAmountValue(param); break;
	case UseItemData::PERCENT: CalcAmountPercent(param); break;
	}

	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		switch (param->base->type)
		{
		case ItemData::DAMAGE:
			mHPAmount = DamageCalculator::CalcItemDamage(mHPAmount, mTargetCharas[i]->GetStatus());

			mTargetCharas[i]->GetStatus()->SubHP(mHPAmount);
			mTargetCharas[i]->GetStatus()->SubMP(mMPAmount);
			mSound = Sound::BOMB;
			break;

		case ItemData::HEAL:
			mTargetCharas[i]->GetStatus()->AddHP(mHPAmount);
			mTargetCharas[i]->GetStatus()->AddMP(mMPAmount);
			mSound = Sound::HEAL;
			break;
		}
	}


	// �G�t�F�N�g����
	switch (param->base->type)
	{
	case ItemData::DAMAGE:
		mEffectSlot = TurnManager::ITEM_DAMAGE_EFFECT_SLOT; 
		mFontRGB = DAMAGE_RGB;
		break;
	case ItemData::HEAL: 
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

	// �A�C�e�������炷
	mMoveChara->GetInventory()->Sub(param->base->id);
	++mState;
}

void ProductionUseItem::StateUseItemWait()
{
	// ���[�V�������I�������
	if (mMoveChara->IsMotionFinished())
	{
		// �G�t�F�N�g�Đ�
		for (size_t i = 0; i < mTargetCharas.size(); ++i)
		{
			Vector3 effectPos = mTargetCharas[i]->GetPos();
			mEffectInstHandles.push_back(Singleton<EffectManager>().GetInstance().Play(mEffectSlot, effectPos, 0, 1.0f, 2.0f));
			AUDIO.SoundPlay((int)mSound);
			++mState;
		}
	}
}

void ProductionUseItem::StateWaitEffect()
{
	std::vector<bool> isPlay;
	isPlay.resize(mTargetCharas.size());
	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		isPlay[i] = Singleton<EffectManager>().GetInstance().IsPlay(mEffectInstHandles[i]);
	}

	// �I�����Ă��Ȃ��G�t�F�N�g������Ώ������Ȃ�
	if(std::find(isPlay.begin(), isPlay.end(), false) == isPlay.end()) return; 

	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		// ����ł��玀�S�G�t�F�N�g
		if (mTargetCharas[i]->GetStatus()->IsDead())
		{
			// Exist��false�ɂ��āA�G�t�F�N�g���Đ�����
			mTargetCharas[i]->SetExist(false); // �����Ȃ�����
			Vector3 effectPos(mTargetCharas[i]->GetPos().x, mTargetCharas[i]->GetPos().y + mTargetCharas[i]->GetLocalAABB().max.y * 0.5f, mTargetCharas[i]->GetPos().z);
			Singleton<EffectManager>().GetInstance().Play(TurnManager::DEATH_EFFECT_SLOT, effectPos);// �����ӂ�����
		}

		// �_���[�W�A�񕜂̕����Z�b�g
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

void ProductionUseItem::CalcAmountValue(const UseItemData::Param* param)
{
	mHPAmount = param->hpValue;
	mMPAmount = param->mpValue;
}

void ProductionUseItem::CalcAmountPercent(const UseItemData::Param* param)
{
	// hpValue��%�Ȃ̂ŁA100�Ŋ���
	const float MAX_PERCENT = 100.0f;
	mHPAmount = (int)(mMoveChara->GetStatus()->GetMaxHP() * (param->hpValue / MAX_PERCENT));
	mMPAmount = (int)(mMoveChara->GetStatus()->GetMaxMP() * (param->mpValue / MAX_PERCENT));
}

