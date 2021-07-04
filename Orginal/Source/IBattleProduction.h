#pragma once
#include "ProductionValue.h"

class BattleActor;

class IBattleProduction
{
protected:
	// int moveActorID;
	// int targetActorID;
	BattleActor* mMoveActor = nullptr;	
	BattleActor* mTargetActor = nullptr;
	int mAmount = -1;

	ProductionValue mProductionValue;

public:
	IBattleProduction() = default;
	~IBattleProduction() = default;

	void Begin(BattleActor* moveActor, BattleActor* targetActor, const int amount);

	virtual void Initialize() = 0;
	virtual int Update() = 0;		// ñﬂÇËílÅFmProductionDataÇÃamount
	virtual void Render() = 0;
};