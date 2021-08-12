#pragma once
#include "ProductionValue.h"

class BattleCharacter;
class BattleCharacterManager;

class IBattleProduction
{
protected: // •¶š‚ÌF
	static const Vector3 DAMAGE_RGB;
	static const Vector3 HEAL_HP_RGB;
	static const Vector3 HEAL_MP_RGB;

protected:
	int mMoveCharaID = -1;
	BattleCharacter* mMoveChara = nullptr;	 // ID‚©‚çæ“¾‚µ‚½‚Ì‚ğ•Û‘¶‚·‚é‚æ‚¤
	std::vector<int> mTargetCharaIDs;
	std::vector<BattleCharacter*> mTargetCharas; // “¯ã

	ProductionValue mProductionValue;
	bool mIsFinished = false;
public:
	IBattleProduction() = default;
	virtual ~IBattleProduction() = default;

	void Begin(const int moveCharaID, const std::vector<int>& targetCharaIDs);

	virtual void Initialize() = 0;
	virtual void Update(const BattleCharacterManager* bcm) = 0;
	virtual void Render() {};

	bool IsFinished() const { return mIsFinished; }
};