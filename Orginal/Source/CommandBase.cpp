#include "CommandBase.h"

void CommandBase::BehaviourFinished()
{
	mBehaviour = Behaviour::NONE;
	mUseItemParam = nullptr;
	mUseSkillParam = nullptr;
	mTargetObjIDs.clear();
}
