#pragma once
#include "ProductionValue.h"

class BattleCharacter;
class BattleCharacterManager;

class IBattleProduction
{
protected: // 文字の色
	static const Vector3 DAMAGE_RGB;
	static const Vector3 HEAL_HP_RGB;
	static const Vector3 HEAL_MP_RGB;

protected:
	 int mMoveCharaID;
	 int mTargetCharaID;
	BattleCharacter* mMoveChara = nullptr;	 // IDから取得したのを保存するよう
	BattleCharacter* mTargetChara = nullptr; // 同上
	//int mAmount = -1;

	ProductionValue mProductionValue;
	bool mIsFinished = false;
public:
	IBattleProduction() = default;
	~IBattleProduction() = default;

	void Begin(const int moveCharaID, const int targetCharaID);

	virtual void Initialize() = 0;
	virtual void Update(const BattleCharacterManager* bcm) = 0;
	virtual void Render() {};

	bool IsFinished() const { return mIsFinished; }
};