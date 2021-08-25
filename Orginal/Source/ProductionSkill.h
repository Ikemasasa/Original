#pragma once
#include "IBattleProduction.h"

class ProductionSkill : public IBattleProduction
{
	enum State
	{
		INIT,
		MOTION_WAIT,
		EFFECT_WAIT,
		WAIT

	};
	int mState;

	void StateInit(const BattleCharacterManager* bcm);


	int CalcAmountValue(float* value, const BattleCharacterManager* bcm);

public:
	ProductionSkill() = default;
	~ProductionSkill() = default;

	void Initialize() override;
	void Update(const BattleCharacterManager* bcm) override;
	void Render() override;
};