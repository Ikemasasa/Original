#include "ProductionUseItem.h"

#include "BattleCharacterManager.h"
#include "CameraManager.h"
#include "CommandBase.h"
#include "DamageCalculator.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "Item.h"
#include "Singleton.h"
#include "Sound.h"
#include "TurnManager.h"

void ProductionUseItem::Initialize()
{
	mProductionValue.Initialize();
}

void ProductionUseItem::Update()
{
	switch (mState)
	{
	case State::INIT: // �����ݒ�

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
}

void ProductionUseItem::Render()
{
	mProductionValue.Render();
}

void ProductionUseItem::StateInit()
{
	// �A�C�e���g�p���[�V�����Z�b�g
	mMoveChara->SetMotionOnce(Character::USE_ITEM, Character::IDLE);

	// �g�p�A�C�e���擾
	const UseItemData::Param* param = mMoveChara->GetCommand()->GetItemParam();

	// ���ʗʌv�Z(mAmount�ɑ�������) 
	switch (param->rate)
	{
	case UseItemData::VALUE:   CalcAmountValue(param); break;
	case UseItemData::PERCENT: CalcAmountPercent(param); break;
	}

	// �_���[�W�v�Z
	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		switch (param->base->type)
		{
		case ItemData::DAMAGE:
			mHPAmounts[i] = DamageCalculator::CalcItemDamage(mHPAmounts[i], mTargetCharas[i]->GetStatus());

			mTargetCharas[i]->GetStatus()->SubHP(mHPAmounts[i]);
			mTargetCharas[i]->GetStatus()->SubMP(mMPAmounts[i]);
			break;

		case ItemData::HEAL:
			mTargetCharas[i]->GetStatus()->AddHP(mHPAmounts[i]);
			mTargetCharas[i]->GetStatus()->AddMP(mMPAmounts[i]);
			break;
		}
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
		// �A�C�e�����擾
		const UseItemData::Param* param = mMoveChara->GetCommand()->GetItemParam();

		// �T�E���h�A�G�t�F�N�g�擾
		Sound::Kind sound = (Sound::Kind)param->soundSlot;
		EffectData::Kind effect = (EffectData::Kind)param->effectSlot;

		// �^�[�Q�b�g�L�����S�{��
		for (const auto& target : mTargetCharas)
		{
			if (param->base->type == ItemData::DAMAGE)
			{
				// 1�t���[���œ��Ă邽�߁A�����蔻��Ƃ��͂���Ȃ����ǁA
				// HitProduction�ɍ��킹�邽�߂�SPHERE�����
				Vector3 center = target->GetTargetPos();
				float radius = target->GetCapsule().radius;
				SPHERE sphere = { center, radius };

				IBattleProduction::HitProduction(sphere, effect, sound);
			}

			if (param->base->type == ItemData::HEAL)
			{
				IBattleProduction::HealProduction(effect, sound);
			}

		}

		++mState;
	}
}

void ProductionUseItem::StateWaitEffect()
{
	// �G�t�F�N�g���Đ������`�F�b�N
	bool isPlay = false;
	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		if (Singleton<EffectManager>().GetInstance().IsPlay(mEffectInstHandles[i]))
		{
			isPlay = true;
			break;
		}
	}

	// �S�ďI������Ύ��̃X�e�[�g
	if (!isPlay)
	{
		++mState;
	}
}

void ProductionUseItem::CalcAmountValue(const UseItemData::Param* param)
{
	for (const auto& target : mTargetCharas)
	{
		mHPAmounts.push_back(param->hpValue);
		mMPAmounts.push_back(param->mpValue);
	}
}

void ProductionUseItem::CalcAmountPercent(const UseItemData::Param* param)
{
	// hpValue��%�Ȃ̂ŁA100�Ŋ���
	const float MAX_PERCENT = 100.0f;
	int amountHP = (int)(mMoveChara->GetStatus()->GetMaxHP() * (param->hpValue / MAX_PERCENT));
	int amountMP = (int)(mMoveChara->GetStatus()->GetMaxMP() * (param->mpValue / MAX_PERCENT));

	mHPAmounts.push_back(amountHP);
	mMPAmounts.push_back(amountMP);
}

