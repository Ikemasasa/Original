#include "ProductionSkill.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "DataBase.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "SkillData.h"
#include "Singleton.h"
#include "StatusData.h"

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
		mEffectInstHandles.resize(mTargetCharaIDs.size());

		StateInit();

		mMoveChara->SetMotion(SkinnedMesh::USE_ITEM, false);

		++mState;
		//break;

	case MOTION_WAIT:
		if (mMoveChara->IsMotionFinished())
		{
			int index = 0;
			for (auto target : mTargetCharas)
			{
				int handle = Singleton<EffectManager>().GetInstance().Play(TurnManager::ATK_BUFF_EFFECT_SLOT, target->GetPos(), 0, 1.0f, 2.5f);
				mEffectInstHandles[index] = handle;
				++index;
			}

			++mState;
		}
		break;

	case EFFECT_WAIT:
	{
		bool isEffectFinished = true;
		for (size_t i = 0; i < mEffectInstHandles.size(); ++i)
		{
			int handle = mEffectInstHandles[i];
			if (Singleton<EffectManager>().GetInstance().IsPlay(handle))
			{
				isEffectFinished = false;
			}
		}

		// エフェクトが終わったら
		if (isEffectFinished)
		{
			++mState;
		}
	}

		break;

	case WAIT:
		mTimer += GameManager::elapsedTime;
		if (mTimer >= WAIT_SEC)
		{
			mIsFinished = true;
			mTimer = 0.0f;
		}
		break;
	}
}

void ProductionSkill::Render()
{
}

void ProductionSkill::StateInit()
{
	// 使用アイテム取得
	const SkillData::SkillParam* param = mMoveChara->GetCommand()->GetSkillParam();

	// バフセット
	for (auto& target : mTargetCharas)
	{
		target->GetStatus()->SetBuffAtkRate(param->atkValue, param->turn);
		target->GetStatus()->SubMP(param->useMP);
		Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(target->GetCharaID(), *target->GetStatus());
	}
}

std::vector<float> ProductionSkill::CalcAmountValue(const SkillData::SkillParam* param)
{
	std::vector<float> ret;
	for (auto& target : mTargetCharas)
	{
		Status* status = target->GetStatus();
		float rate = 1.0f + param->atkValue / status->GetStr();
		ret.push_back(rate);
	}

	return ret;
}
