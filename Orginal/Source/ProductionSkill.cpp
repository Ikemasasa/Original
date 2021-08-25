#include "ProductionSkill.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "SkillData.h"

void ProductionSkill::Initialize()
{
}

void ProductionSkill::Update(const BattleCharacterManager* bcm)
{
	switch (mState)
	{
	case INIT:
		mMoveChara = bcm->GetChara(mMoveCharaID);
		for (size_t i = 0; i < mTargetCharaIDs.size(); ++i)
		{
			mTargetCharas.push_back(bcm->GetChara(mTargetCharaIDs[i]));
		}

		mMoveChara->SetMotion(SkinnedMesh::USE_ITEM);

		++mState;
		//break;

	case MOTION_WAIT:
		if (mMoveChara->IsMotionFinished())
		{
			++mState;
		}
	}
}

void ProductionSkill::Render()
{
}

void ProductionSkill::StateInit(const BattleCharacterManager* bcm)
{
	// 使用アイテム取得
	const SkillData::SkillParam* param = mMoveChara->GetCommand()->GetSkillParam();

	for (auto& targetID : mTargetCharaIDs)
	{
		bcm->GetChara(targetID)->GetStatus()->SetBuffStrRate(param->atkValue, param->turn);
	}

	for (size_t i = 0; i < mTargetCharas.size(); ++i)
	{
		switch (param->type)
		{
		case ItemData::Effect::DAMAGE:
			mHPAmount = DamageCalculator::CalcItemDamage(mHPAmount, mTargetCharas[i]->GetStatus());

			mTargetCharas[i]->GetStatus()->SubHP(mHPAmount);
			mTargetCharas[i]->GetStatus()->SubMP(mMPAmount);
			break;

		case ItemData::Effect::HEAL:
			mTargetCharas[i]->GetStatus()->AddHP(mHPAmount);
			mTargetCharas[i]->GetStatus()->AddMP(mMPAmount);
			break;
		}
	}
}

int ProductionSkill::CalcAmountValue(float* value, const BattleCharacterManager* bcm)
{
	for (auto& targetID : mTargetCharaIDs)
	{
		Status* status = bcm->GetChara(targetID)->GetStatus();
		float rate = 1.0f + *value / status->GetStr();
		*value = rate;
	}
}
