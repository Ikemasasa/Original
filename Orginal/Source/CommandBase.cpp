#include "CommandBase.h"

void CommandBase::BehaviourFinished()
{
	mBehaviour = Behaviour::NONE;
	mUseItemIndex = -1;
	mTargetObjID = -1;
}
