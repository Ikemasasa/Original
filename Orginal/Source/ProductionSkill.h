#pragma once
#include <vector>

#include "IBattleProduction.h"
#include "SkillData.h"

class ProductionSkill : public IBattleProduction
{
	static constexpr float WAIT_SEC = 1.0f;

	enum State
	{
		INIT,
		MOTION_WAIT,
		EFFECT_WAIT,
		WAIT

	};
	int mState = INIT;
	float mTimer = 0.0f;
	std::vector<int> mEffectInstHandles;

	void StateInit();

	std::vector<float> CalcAmountValue(const SkillData::SkillParam* param);

public:
	ProductionSkill() = default;
	~ProductionSkill() = default;

	void Initialize() override;
	void Update(const BattleCharacterManager* bcm) override;
	void Render() override;
};