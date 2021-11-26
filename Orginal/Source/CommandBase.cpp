#include "CommandBase.h"

void CommandBase::BehaviourFinished()
{
	// �e��p�����[�^�����Z�b�g
	mBehaviour = Behaviour::NONE;
	mIsUseItem = false;
	mIsUseSkill = false;
	mTargetObjIDs.clear();
}

void CommandBase::SetItemParam(const UseItemData::Param* param)
{
	// null�Ȃ�A�C�e����s�g�p�ɂ���
	if (!param)
	{
		mIsUseItem = false;
		return;
	}

	// �p�����[�^�ݒ�
	mUseItemParam = *param;
	mIsUseItem = true;
}

void CommandBase::SetSkillParam(const SkillData::BaseData* param)
{
	// null�Ȃ�X�L����s�g�p�ɂ���
	if (!param)
	{
		mIsUseSkill = false;
		return;
	}

	// �p�����[�^�ݒ�
	mUseSkillParam = *param;
	mIsUseSkill = true;
}