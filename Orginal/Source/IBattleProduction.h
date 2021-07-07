#pragma once
#include "ProductionValue.h"

class BattleActor;
class BattleActorManager;

class IBattleProduction
{
protected:
	 int mMoveActorID;
	 int mTargetActorID;
	BattleActor* mMoveActor = nullptr;	 // ID����擾�����̂�ۑ�����悤
	BattleActor* mTargetActor = nullptr; // ����
	//int mAmount = -1;

	ProductionValue mProductionValue;
	bool mIsFinished = false;
public:
	IBattleProduction() = default;
	~IBattleProduction() = default;

	void Begin(const int moveActorID, const int targetActorID);

	virtual void Initialize() = 0;
	virtual void Update(const BattleActorManager* bam) = 0;
	virtual void Render() = 0;

	bool GetIsFinished() const { return mIsFinished; }
};