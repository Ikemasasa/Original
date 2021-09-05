#include "CommandBase.h"

void CommandBase::BehaviourFinished()
{
	mBehaviour = Behaviour::NONE;
	mIsUseItem = false;
	mIsUseSkill = false;
	mTargetObjIDs.clear();
}

void CommandBase::SetItemParam(const UseItemData::Param* param)
{
	if (!param)
	{
		mIsUseItem = false;
		return;
	}

	mUseItemParam = *param;
	mIsUseItem = true;
}

void CommandBase::SetSkillParam(const SkillData::SkillParam* param)
{
	if (!param)
	{
		mIsUseSkill = false;
		return;
	}

	mUseSkillParam = *param;
	mIsUseSkill = true;
}