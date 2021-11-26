#pragma once
#include <vector>

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

	int mState = INIT;

	void StateInit();
	void StateWaitMotion();
	void StateWaitEffect();

public:
	ProductionSkill() = default;
	~ProductionSkill() = default;

	void Initialize() override;
	void Update() override;
	void Render() override;
};