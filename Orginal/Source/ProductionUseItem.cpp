#include "ProductionUseItem.h"

#include "BattleActorManager.h"
#include "CommandBase.h"
#include "GameManager.h"
#include "Singleton.h"
#include "EffectManager.h"


void ProductionUseItem::Initialize()
{
	mProductionValue.Initialize();
}

void ProductionUseItem::Update(const BattleActorManager* bam)
{
	switch (mState)
	{
	case State::INIT: // �����ݒ�
	{
		// ID����|�C���^�擾
		mMoveActor = bam->GetActor(mMoveActorID);
		mTargetActor = bam->GetActor(mTargetActorID);
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
}

void ProductionUseItem::Render()
{
	mProductionValue.Render();
}

void ProductionUseItem::StateInit()
{
	// �A�C�e���g�p���[�V�����Z�b�g
	mMoveActor->SetMotion(SkinnedMesh::USE_ITEM, false);

	// �g�p�A�C�e���Z�b�g
	mUseItemParam = *mMoveActor->GetCommand()->GetItemParam();

	// ���ʗʌv�Z(m~Amount�ɑ�������) 
	switch (mUseItemParam.rate)
	{
	case ItemData::VALUE:  CalcAmountValue(); break;
	case ItemData::PERCENT: CalcAmountPercent(); break;
	}

	// �_���[�W�A�C�e����hpvalue�̓}�C�i�X�ɂȂ��Ă邩��q�[���ŉ񕜁A�_���[�W�����ł���
	mTargetActor->GetStatus()->HealHP(mHPAmount);
	mTargetActor->GetStatus()->HealMP(mMPAmount);

	// �G�t�F�N�g����
	//if (mHPAmount < 0) mEffectSlot = TurnManager::ITEM_DAMAGE_EFFECT; // TODO ������
	if (mMPAmount > 0) mEffectSlot = TurnManager::MAGIC_POTION_EFFECT_SLOT;
	else if (mHPAmount > 0) mEffectSlot = TurnManager::HEAL_POTION_EFFECT_SLOT;


	++mState;
}

void ProductionUseItem::StateUseItemWait()
{
	// ���[�V�������I�������
	if (mMoveActor->IsMotionFinished())
	{
		// �G�t�F�N�g�Đ�
		Vector3 tp = mTargetActor->GetPos();
		float aabbYDist = mTargetActor->GetLocalAABB().max.y - mTargetActor->GetLocalAABB().min.y;
		Vector3 effectPos = Vector3(tp.x, tp.y + (aabbYDist / 2.0f), tp.z);
		mEffectInstHandle = Singleton<EffectManager>().GetInstance().Play(mEffectSlot, effectPos);
		++mState;
	}
}

void ProductionUseItem::StateWaitEffect()
{
	bool isPlay = Singleton<EffectManager>().GetInstance().IsPlay(mEffectInstHandle);

	// �G�t�F�N�g�̍Đ������������ 
	if (!isPlay)
	{
		++mState;
	}
}

void ProductionUseItem::StateWait()
{
	static float waitTimer = 0.0f;
	waitTimer += GameManager::elpsedTime;

	const float WAIT_SEC = 1.5f;
	if (waitTimer >= WAIT_SEC)
	{
		mState = 0;
		waitTimer = 0.0f;
		mIsFinished = true;
	}
}

void ProductionUseItem::CalcAmountValue()
{
	mHPAmount = mUseItemParam.hpValue;
	mMPAmount = mUseItemParam.hpValue;
}

void ProductionUseItem::CalcAmountPercent()
{
	// hpValue��%�Ȃ̂ŁA100�Ŋ���
	const float MAX_PERCENT = 100.0f;
	mHPAmount = mMoveActor->GetStatus()->maxHP * (mUseItemParam.hpValue / MAX_PERCENT);
	mMPAmount = mMoveActor->GetStatus()->maxMP * (mUseItemParam.mpValue / MAX_PERCENT);
}

