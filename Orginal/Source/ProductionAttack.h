#pragma once
#include "IBattleProduction.h"

#include "lib/Vector.h"

struct Status;

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

private:
	void StateInit();
	void StateMoveToTarget();
	void StateWaitAttack();
	void StateMoveToOrigin();
	void StateWait();
	int CalcDamage(const Status* deal, Status* take);

public:
	void Initialize() override;
	void Update(const BattleActorManager* bam) override;
	void Render() override;
};