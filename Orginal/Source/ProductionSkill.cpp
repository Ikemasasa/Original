#include "ProductionSkill.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "DataBase.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "SkillData.h"
#include "Singleton.h"
#include "StatusData.h"
#include "DamageCalculator.h"

void ProductionSkill::Initialize()
{
	mProductionValue.Initialize();
}

void ProductionSkill::Update()
{
	switch (mState)
	{
	case INIT:
		// モーション変更
		mMoveChara->SetMotionOnce(Character::USE_ITEM, Character::IDLE);
		
		StateInit();
		++mState;
		//break;

	case MOTION_WAIT:
		StateWaitMotion();
		break;

	case EFFECT_WAIT:
		StateWaitEffect();
		break;

	case WAIT:
		StateWait();
		break;
	}
}

void ProductionSkill::Render()
{
	mProductionValue.Render();
}

void ProductionSkill::StateInit()
{
	// 使用スキル情報取得
	const SkillData::BaseData* baseParam = mMoveChara->GetCommand()->GetSkillParam();
	
	// 使用キャラのMPを減らす
	mMoveChara->GetStatus()->SubMP(baseParam->useMP);

	// スキル効果反映
	for (auto& target : mTargetCharas)
	{
		// バフ
		if (baseParam->type == SkillData::BUFF)
		{
			BuffSkillData::Param param = BuffSkillData::GetParam(baseParam->id);
			if (target == mMoveChara) param.turn += 1; // 自分に対してのバフなら1ターン追加(バフターン経過のあれやそれで)
			if (param.atkValue != 0.0f) target->GetStatus()->SetBuffAtkRate(param.atkValue, param.turn);
			if (param.defValue != 0.0f) target->GetStatus()->SetBuffDefRate(param.defValue, param.turn);
		}

		// デバフ
		if (baseParam->type == SkillData::DEBUFF)
		{
			BuffSkillData::Param param = BuffSkillData::GetParam(baseParam->id);
			if (target == mMoveChara) param.turn += 1; // 自分に対してのデバフなら1ターン追加(バフターン経過のあれやそれで)
			if (param.atkValue != 0.0f) target->GetStatus()->SetDebuffAtkRate(param.atkValue, param.turn);
			if (param.defValue != 0.0f) target->GetStatus()->SetDebuffDefRate(param.defValue, param.turn);
		}

		// 攻撃(現在は単体1ヒット対応
		if (baseParam->type == SkillData::ATTACK)
		{
			AttackSkillData::Param param = AttackSkillData::GetParam(baseParam->id);
			std::vector<int> amount = DamageCalculator::CalcSkillDamage(mMoveChara->GetStatus(), target->GetStatus(), param.skillRate, param.hitNum);
			for (const auto& a : amount)
			{
				target->GetStatus()->SubHP(a);
				mHPAmounts.push_back(a);
			}
		}
	}
}

void ProductionSkill::StateWaitMotion()
{
	// モーションが終わったら
	if (mMoveChara->IsMotionFinished())
	{
		// 使用スキル情報を取得
		const SkillData::BaseData* baseParam = mMoveChara->GetCommand()->GetSkillParam();

		// エフェクト、サウンドのパラメータ取得
		EffectData::Param efcParam = EffectData::GetParam((EffectData::Kind)baseParam->effectID);
		Sound::Kind sound = (Sound::Kind)baseParam->soundID;

		if (baseParam->type == SkillData::ATTACK)
		{
			IBattleProduction::AtkSkillProduction((EffectData::Kind)efcParam.slotID, sound);
		}
		if (baseParam->type == SkillData::BUFF || baseParam->type == SkillData::DEBUFF)
		{
			IBattleProduction::BuffProduction((EffectData::Kind)efcParam.slotID, sound);
		}



		++mState;
	}
}

void ProductionSkill::StateWaitEffect()
{
	// 全てのエフェクトの再生が終了したかチェック
	bool isPlay = false;
	for (const auto& handle : mEffectInstHandles)
	{
		if (Singleton<EffectManager>().GetInstance().IsPlay(handle))
		{
			isPlay = true;
			break;
		}
	}

	// エフェクトが終わったら
	if (!isPlay)
	{
		// 攻撃スキルならヒットエフェクト
		const SkillData::BaseData* base = mMoveChara->GetCommand()->GetSkillParam();
		if (base->type == SkillData::ATTACK)
		{
			for (const auto& target : mTargetCharas)
			{
				SPHERE sphere = { target->GetTargetPos(), target->GetCapsule().radius };
				IBattleProduction::HitProduction(sphere, EffectData::DAMAGE, Sound::ATTACK_HIT);
			}
		}

		++mState;
	}
}
