#include "ProductionUseItem.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "DamageCalculator.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "Item.h"
#include "Singleton.h"


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
		for (int i = 0; i < mTargetCharaIDs.size(); ++i)
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
	const ItemData::ItemParam* param = mMoveChara->GetInventory()->GetItemParam(mMoveChara->GetCommand()->GetItemIndex());

	// ���ʗʌv�Z(m~Amount�ɑ�������) 
	switch (param->rate)
	{
	case ItemData::VALUE:  CalcAmountValue(param); break;
	case ItemData::PERCENT: CalcAmountPercent(param); break;
	}

	for (int i = 0; i < mTargetCharas.size(); ++i)
	{
		// �_���[�W�A�C�e����hpvalue�̓}�C�i�X�ɂȂ��Ă邩��q�[���ŉ񕜁A�_���[�W�����ł���
		if (mHPAmount < 0) mHPAmount = DamageCalculator::CalcItemDamage(mHPAmount, mTargetCharas[i]->GetStatus());
		mTargetCharas[i]->GetStatus()->HealHP(mHPAmount);
		mTargetCharas[i]->GetStatus()->HealMP(mMPAmount);
		mMoveChara->GetInventory()->Sub(mMoveChara->GetCommand()->GetItemIndex());
	}


	// �G�t�F�N�g����
	if (mHPAmount < 0) mEffectSlot = TurnManager::ITEM_DAMAGE_EFFECT_SLOT;
	else if (mMPAmount > 0) mEffectSlot = TurnManager::MAGIC_POTION_EFFECT_SLOT;
	else if (mHPAmount > 0) mEffectSlot = TurnManager::HEAL_POTION_EFFECT_SLOT;


	++mState;
}

void ProductionUseItem::StateUseItemWait()
{
	// ���[�V�������I�������
	if (mMoveChara->IsMotionFinished())
	{
		// �G�t�F�N�g�Đ�
		for (int i = 0; i < mTargetCharas.size(); ++i)
		{
			Vector3 effectPos = mTargetCharas[i]->GetPos();
			mEffectInstHandles.push_back(Singleton<EffectManager>().GetInstance().Play(mEffectSlot, effectPos));
			++mState;
		}
	}
}

void ProductionUseItem::StateWaitEffect()
{
	std::vector<bool> isPlay;
	isPlay.resize(mTargetCharas.size());
	for (int i = 0; i < mTargetCharas.size(); ++i)
	{
		isPlay[i] = Singleton<EffectManager>().GetInstance().IsPlay(mEffectInstHandles[i]);
	}

	// �I�����Ă��Ȃ��G�t�F�N�g������Ώ������Ȃ�
	if(std::find(isPlay.begin(), isPlay.end(), false) == isPlay.end()) return; 

	for (int i = 0; i < mTargetCharas.size(); ++i)
	{
		// ����ł��玀�S�G�t�F�N�g
		if (mTargetCharas[i]->GetStatus()->IsDead())
		{
			// Exist��false�ɂ��āA�G�t�F�N�g���Đ�����
			mTargetCharas[i]->SetExist(false); // �����Ȃ�����
			Vector3 effectPos(mTargetCharas[i]->GetPos().x, mTargetCharas[i]->GetPos().y + mTargetCharas[i]->GetLocalAABB().max.y * 0.5f, mTargetCharas[i]->GetPos().z);
			Singleton<EffectManager>().GetInstance().Play(TurnManager::DEATH_EFFECT_SLOT, effectPos);// �����ӂ�����
		}

		// HP�̕����Z�b�g
		int addNum = 0;
		const float ADDJUST_Y = 0.1f;
		Vector3 pos(mTargetCharas[i]->GetPos().x, mTargetCharas[i]->GetAABB().max.y - ADDJUST_Y * addNum, mTargetCharas[i]->GetPos().z);
		if (mHPAmount > 0)
		{
			mProductionValue.Add(mHPAmount, pos, HEAL_HP_RGB);
			++addNum;
		}
		else if (mHPAmount < 0)
		{
			mProductionValue.Add(-mHPAmount, pos, DAMAGE_RGB); // -��`�悳���Ȃ�����-mHPAmount�ɂ��Ă�
			++addNum;
		}

		// MP�̕����Z�b�g
		if (mMPAmount > 0)
		{
			mProductionValue.Add(mHPAmount, pos, HEAL_HP_RGB);
			++addNum;
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

void ProductionUseItem::CalcAmountValue(const ItemData::ItemParam* param)
{
	mHPAmount = param->hpValue;
	mMPAmount = param->mpValue;
}

void ProductionUseItem::CalcAmountPercent(const ItemData::ItemParam* param)
{
	// hpValue��%�Ȃ̂ŁA100�Ŋ���
	const float MAX_PERCENT = 100.0f;
	mHPAmount = (int)(mMoveChara->GetStatus()->maxHP * (param->hpValue / MAX_PERCENT));
	mMPAmount = (int)(mMoveChara->GetStatus()->maxMP * (param->mpValue / MAX_PERCENT));
}

