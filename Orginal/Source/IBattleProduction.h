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
	int mMoveCharaID = -1;
	BattleCharacter* mMoveChara = nullptr;	 // IDから取得したのを保存するよう
	std::vector<int> mTargetCharaIDs;
	std::vector<BattleCharacter*> mTargetCharas; // 同上
	std::vector<BattleCharacter*> mHitChara; // コリジョン用
	std::vector<int> mAmounts;
	ProductionValue mProductionValue;
	bool mIsFinished = false;

protected:

public:
	IBattleProduction() = default;
	virtual ~IBattleProduction() = default;

	void Begin(const int moveCharaID, const std::vector<int>& targetCharaIDs);
	void UpdateDeathMotion();

	virtual void Initialize() = 0;
	virtual void Update(const BattleCharacterManager* bcm) = 0;
	virtual void Render() {};

	bool IsFinished() const { return mIsFinished; }
};