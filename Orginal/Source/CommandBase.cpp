#include "CommandBase.h"

void CommandBase::BehaviourFinished()
{
	// 各種パラメータをリセット
	mBehaviour = Behaviour::NONE;
	mIsUseItem = false;
	mIsUseSkill = false;
	mTargetObjIDs.clear();
}

void CommandBase::SetItemParam(const UseItemData::Param* param)
{
	// nullならアイテムを不使用にする
	if (!param)
	{
		mIsUseItem = false;
		return;
	}

	// パラメータ設定
	mUseItemParam = *param;
	mIsUseItem = true;
}

void CommandBase::SetSkillParam(const SkillData::BaseData* param)
{
	// nullならスキルを不使用にする
	if (!param)
	{
		mIsUseSkill = false;
		return;
	}

	// パラメータ設定
	mUseSkillParam = *param;
	mIsUseSkill = true;
}