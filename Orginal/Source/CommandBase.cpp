#include "CommandBase.h"

void CommandBase::BehaviourFinished()
{
	mBehaviour = Behaviour::NONE;
	mUseItemParam = nullptr;
	mIsUseSkill = false;
	mTargetObjIDs.clear();
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