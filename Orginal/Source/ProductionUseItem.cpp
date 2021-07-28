#include "ProductionUseItem.h"

#include "BattleActorManager.h"
#include "CommandBase.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "Item.h"
#include "Singleton.h"


void ProductionUseItem::Initialize()
{
	mProductionValue.Initialize();
}

void ProductionUseItem::Update(const BattleActorManager* bam)
{
	switch (mState)
	{
	case State::INIT: // �����ݒ�

		// ID����|�C���^�擾
		mMoveActor = bam->GetActor(mMoveActorID);
		mTargetActor = bam->GetActor(mTargetActorID);

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
	mMoveActor->SetMotion(SkinnedMesh::USE_ITEM, false);

	// �g�p�A�C�e���擾
	const ItemData::ItemParam* param = mMoveActor->GetInventory()->GetItemParam(mMoveActor->GetCommand()->GetItemIndex());

	// ���ʗʌv�Z(m~Amount�ɑ�������) 
	switch (param->rate)
	{
	case ItemData::VALUE:  CalcAmountValue(param); break;
	case ItemData::PERCENT: CalcAmountPercent(param); break;
	}

	// �_���[�W�A�C�e����hpvalue�̓}�C�i�X�ɂȂ��Ă邩��q�[���ŉ񕜁A�_���[�W�����ł���
	mTargetActor->GetStatus()->HealHP(mHPAmount);
	mTargetActor->GetStatus()->HealMP(mMPAmount);
	mMoveActor->GetInventory()->Sub(mMoveActor->GetCommand()->GetItemIndex());

	// �G�t�F�N�g����
	if (mHPAmount < 0) mEffectSlot = TurnManager::ITEM_DAMAGE_EFFECT_SLOT;
	else if (mMPAmount > 0) mEffectSlot = TurnManager::MAGIC_POTION_EFFECT_SLOT;
	else if (mHPAmount > 0) mEffectSlot = TurnManager::HEAL_POTION_EFFECT_SLOT;


	++mState;
}

void ProductionUseItem::StateUseItemWait()
{
	// ���[�V�������I�������
	if (mMoveActor->IsMotionFinished())
	{
		// �G�t�F�N�g�Đ�
		Vector3 effectPos = mTargetActor->GetPos();
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
		// ����ł��玀�S�G�t�F�N�g
		if (mTargetActor->GetStatus()->IsDead())
		{
			// Exist��false�ɂ��āA�G�t�F�N�g���Đ�����
			mTargetActor->SetExist(false); // �����Ȃ�����
			Vector3 effectPos(mTargetActor->GetPos().x, mTargetActor->GetPos().y + mTargetActor->GetLocalAABB().max.y * 0.5f, mTargetActor->GetPos().z);
			Singleton<EffectManager>().GetInstance().Play(TurnManager::DEATH_EFFECT_SLOT, effectPos);// �����ӂ�����
		}

		// HP�̕����Z�b�g
		int addNum = 0;
		const float ADDJUST_Y = 0.1f;
		if (mHPAmount > 0)
		{
			Vector3 pos(mTargetActor->GetPos().x, mTargetActor->GetAABB().max.y - ADDJUST_Y * addNum, mTargetActor->GetPos().z);
			mProductionValue.Add(mHPAmount, pos, HEAL_HP_RGB);
			++addNum;
		}
		else if (mHPAmount < 0)
		{
			Vector3 pos(mTargetActor->GetPos().x, mTargetActor->GetAABB().max.y - ADDJUST_Y * addNum, mTargetActor->GetPos().z);
			mProductionValue.Add(mHPAmount, pos, DAMAGE_RGB);
			++addNum;
		}

		// MP�̕����Z�b�g
		if (mMPAmount > 0)
		{
			Vector3 pos(mTargetActor->GetPos().x, mTargetActor->GetAABB().max.y - ADDJUST_Y * addNum, mTargetActor->GetPos().z);
			mProductionValue.Add(mHPAmount, pos, DAMAGE_RGB);
			++addNum;
		}

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

void ProductionUseItem::CalcAmountValue(const ItemData::ItemParam* param)
{
	mHPAmount = param->hpValue;
	mMPAmount = param->mpValue;
}

void ProductionUseItem::CalcAmountPercent(const ItemData::ItemParam* param)
{
	// hpValue��%�Ȃ̂ŁA100�Ŋ���
	const float MAX_PERCENT = 100.0f;
	mHPAmount = mMoveActor->GetStatus()->maxHP * (param->hpValue / MAX_PERCENT);
	mMPAmount = mMoveActor->GetStatus()->maxMP * (param->mpValue / MAX_PERCENT);
}

