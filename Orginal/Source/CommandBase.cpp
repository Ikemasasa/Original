#include "CommandBase.h"

void CommandBase::BehaviourFinished()
{
	mBehaviour = Behaviour::NONE;
	mUseItemParam = nullptr;
	mTargetObjID = -1;
}
