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
		// ���[�V�����ύX
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
	// �g�p�X�L�����擾
	const SkillData::BaseData* baseParam = mMoveChara->GetCommand()->GetSkillParam();
	
	// �g�p�L������MP�����炷
	mMoveChara->GetStatus()->SubMP(baseParam->useMP);

	// �X�L�����ʔ��f
	for (auto& target : mTargetCharas)
	{
		// �o�t
		if (baseParam->type == SkillData::BUFF)
		{
			BuffSkillData::Param param = BuffSkillData::GetParam(baseParam->id);
			if (target == mMoveChara) param.turn += 1; // �����ɑ΂��Ẵo�t�Ȃ�1�^�[���ǉ�(�o�t�^�[���o�߂̂���₻���)
			if (param.atkValue != 0.0f) target->GetStatus()->SetBuffAtkRate(param.atkValue, param.turn);
			if (param.defValue != 0.0f) target->GetStatus()->SetBuffDefRate(param.defValue, param.turn);
		}

		// �f�o�t
		if (baseParam->type == SkillData::DEBUFF)
		{
			BuffSkillData::Param param = BuffSkillData::GetParam(baseParam->id);
			if (target == mMoveChara) param.turn += 1; // �����ɑ΂��Ẵf�o�t�Ȃ�1�^�[���ǉ�(�o�t�^�[���o�߂̂���₻���)
			if (param.atkValue != 0.0f) target->GetStatus()->SetDebuffAtkRate(param.atkValue, param.turn);
			if (param.defValue != 0.0f) target->GetStatus()->SetDebuffDefRate(param.defValue, param.turn);
		}

		// �U��(���݂͒P��1�q�b�g�Ή�
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
	// ���[�V�������I�������
	if (mMoveChara->IsMotionFinished())
	{
		// �g�p�X�L�������擾
		const SkillData::BaseData* baseParam = mMoveChara->GetCommand()->GetSkillParam();

		// �G�t�F�N�g�A�T�E���h�̃p�����[�^�擾
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
	// �S�ẴG�t�F�N�g�̍Đ����I���������`�F�b�N
	bool isPlay = false;
	for (const auto& handle : mEffectInstHandles)
	{
		if (Singleton<EffectManager>().GetInstance().IsPlay(handle))
		{
			isPlay = true;
			break;
		}
	}

	// �G�t�F�N�g���I�������
	if (!isPlay)
	{
		// �U���X�L���Ȃ�q�b�g�G�t�F�N�g
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
