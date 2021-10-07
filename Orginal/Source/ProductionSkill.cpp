#include "ProductionSkill.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "DataBase.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "SkillData.h"
#include "Singleton.h"
#include "StatusData.h"
#include "TurnManager.h"

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

		mMoveChara->SetMotionOnce(Character::USE_ITEM, Character::IDLE);

		// エフェクト決定
		switch (mMoveChara->GetCommand()->GetSkillParam()->type)
		{
		case SkillData::Type::BUFF:   
			mEffectSlot = TurnManager::BUFF_EFFECT_SLOT;
			mSound = Sound::BUFF;
			break;
		case SkillData::Type::DEBUFF: 
			mEffectSlot = TurnManager::DEBUFF_EFFECT_SLOT;
			mSound = Sound::DEBUFF;
			break;
		}
		++mState;
		//break;

	case MOTION_WAIT:
		if (mMoveChara->IsMotionFinished())
		{
			int index = 0;
			for (auto target : mTargetCharas)
			{
				int handle = Singleton<EffectManager>().GetInstance().Play(mEffectSlot, target->GetPos());
				mEffectInstHandles[index] = handle;
				Audio::SoundPlay((int)mSound);
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

	mMoveChara->GetStatus()->SubMP(param->useMP);

	// バフセット
	for (auto& target : mTargetCharas)
	{
		int turn = param->turn;

		// 自分に対してのバフデバフなら1ターン追加(バフターン経過のあれやそれで)
		if (mMoveChara == target) turn += 1;

		switch (param->type)
		{
		case SkillData::Type::BUFF:
			if (param->atkValue != 0.0f) target->GetStatus()->SetBuffAtkRate(param->atkValue, turn);
			if (param->defValue != 0.0f) target->GetStatus()->SetBuffDefRate(param->defValue, turn);
			break;

		case SkillData::Type::DEBUFF:
			if (param->atkValue != 0.0f) target->GetStatus()->SetDebuffAtkRate(param->atkValue, turn);
			if (param->defValue != 0.0f) target->GetStatus()->SetDebuffDefRate(param->defValue, turn);
			break;
		}
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
