#pragma once
#include "IBattleProduction.h"

#include "lib/Vector.h"

class ProductionAttack : public IBattleProduction
{
	static constexpr float LERP_FACTOR_ADD = 0.03f;
	static constexpr float LERP_FACTOR_MIN = 0.0f;
	static constexpr float LERP_FACTOR_MAX = 1.0f;

	enum State
	{
		INIT,
		MOVE_TO_TARGET,
		WAIT_ATTACK,
		MOVE_TO_ORIGIN,
		WAIT,

		END
	};

	int mState;
	Vector3 mOrgPos;
	Vector3 mDestinationPos;
	float mLerpFactor;
	float mWaitTimer;
	int mDeathEffectSlot;

private:
	void StateInit();
	void StateMoveToTarget();
	void StateWaitAttack();
	void StateMoveToOrigin();
	void StateWait();

public:
	void Initialize() override;
	int Update() override;
	void Render() override;
};