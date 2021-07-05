#pragma once
#include "ProductionValue.h"

class BattleActor;
class BattleActorManager;

class IBattleProduction
{
protected:
	 int mMoveActorID;
	 int mTargetActorID;
	BattleActor* mMoveActor = nullptr;	 // IDから取得したのを保存するよう
	BattleActor* mTargetActor = nullptr; // 同上
	int mAmount = -1;

	ProductionValue mProductionValue;
	bool mIsFinished;
public:
	IBattleProduction() = default;
	~IBattleProduction() = default;

	void Begin(const int moveActorID, const int targetActorID);

	virtual void Initialize() = 0;
	virtual void Update(const BattleActorManager* bam) = 0;		// 戻り値：mProductionDataのamount
	virtual void Render() = 0;

	bool GetIsFinished() const { return mIsFinished; }
};