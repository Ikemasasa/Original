#pragma once
#include <memory>
#include <vector>

#include "lib/Vector.h"

#include "CommandBase.h"
#include "ProductionValue.h"

class BattleActorManager;

class ProductionBattle
{
	static constexpr float LERP_FACTOR_ADD = 0.03f;

	struct Data
	{
		CommandBase::Behaviour behaviour;
		int moveID    = -1;
		int targetID  = -1;
		int amount = -1;
		bool isEnable = false;
	};
	Data mData;

	int mState = 0;
	float mLerpFactor = 0.0f;
	float mTimer = 0.0f;;
	Vector3 mOrgPos;
	Vector3 mDestinationPos;

	ProductionValue mProductionValue;

	int mDeathEffectSlot;
public:
	ProductionBattle();
	~ProductionBattle() = default;

	void Begin(CommandBase::Behaviour behaviour, int moveID, int targetID, int amount);
	
	void Initialize();
	bool Update(const BattleActorManager* bam);
	void Render();
};