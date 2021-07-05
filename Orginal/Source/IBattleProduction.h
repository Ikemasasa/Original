#pragma once
#include "ProductionValue.h"

class BattleActor;
class BattleActorManager;

class IBattleProduction
{
protected:
	 int mMoveActorID;
	 int mTargetActorID;
	BattleActor* mMoveActor = nullptr;	 // ID‚©‚çæ“¾‚µ‚½‚Ì‚ğ•Û‘¶‚·‚é‚æ‚¤
	BattleActor* mTargetActor = nullptr; // “¯ã
	int mAmount = -1;

	ProductionValue mProductionValue;
	bool mIsFinished = false;
public:
	IBattleProduction() = default;
	~IBattleProduction() = default;

	void Begin(const int moveActorID, const int targetActorID);

	virtual void Initialize() = 0;
	virtual void Update(const BattleActorManager* bam) = 0;		// –ß‚è’lFmProductionData‚Ìamount
	virtual void Render() = 0;

	bool GetIsFinished() const { return mIsFinished; }
};