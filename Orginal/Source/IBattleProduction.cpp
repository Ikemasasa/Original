#include "IBattleProduction.h"

void IBattleProduction::Begin(BattleActor* moveActor, BattleActor* targetActor, const int amount)
{
	mMoveActor = moveActor;
	mTargetActor = targetActor;
	mAmount = amount;
}
