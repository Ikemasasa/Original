#pragma once
#include "ProductionValue.h"

class BattleCharacter;
class BattleActorManager;

class IBattleProduction
{
protected: // 文字の色
	static const Vector3 DAMAGE_RGB;
	static const Vector3 HEAL_HP_RGB;
	static const Vector3 HEAL_MP_RGB;

protected:
	 int mMoveActorID;
	 int mTargetActorID;
	BattleCharacter* mMoveActor = nullptr;	 // IDから取得したのを保存するよう
	BattleCharacter* mTargetActor = nullptr; // 同上
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

	bool IsFinished() const { return mIsFinished; }
};