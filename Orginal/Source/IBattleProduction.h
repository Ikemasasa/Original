#pragma once
#include "ProductionValue.h"

class BattleCharacter;
class BattleCharacterManager;

class IBattleProduction
{
protected: // �����̐F
	static const Vector3 DAMAGE_RGB;
	static const Vector3 HEAL_HP_RGB;
	static const Vector3 HEAL_MP_RGB;

protected:
	 int mMoveCharaID;
	 int mTargetCharaID;
	BattleCharacter* mMoveChara = nullptr;	 // ID����擾�����̂�ۑ�����悤
	BattleCharacter* mTargetChara = nullptr; // ����
	//int mAmount = -1;

	ProductionValue mProductionValue;
	bool mIsFinished = false;
public:
	IBattleProduction() = default;
	~IBattleProduction() = default;

	void Begin(const int moveCharaID, const int targetCharaID);

	virtual void Initialize() = 0;
	virtual void Update(const BattleCharacterManager* bcm) = 0;
	virtual void Render() = 0;

	bool IsFinished() const { return mIsFinished; }
};