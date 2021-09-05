#pragma once
#include "IBattleProduction.h"

#include "lib/Vector.h"

struct Status;
class BattleCharacterManager;

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

	int mState = 0;
	Vector3 mOrgPos;
	Vector3 mDestinationPos;
	float mLerpFactor = 0.0f;
	float mWaitTimer = 0;
	std::vector<int> mAmounts;

private:
	void StateInit();
	void StateMoveToTarget();
	void StateWaitAttack();
	void StateMoveToOrigin();
	void StateWait();

public:
	ProductionAttack() = default;
	~ProductionAttack() = default;

	void Initialize() override;
	void Update(const BattleCharacterManager* bcm) override;
	void Render() override;
};