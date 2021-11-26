#pragma once
#include "IBattleProduction.h"

class ProductionEscape : public IBattleProduction
{
	static constexpr float MOVE_SPEED = 0.3f;

	enum State
	{
		INIT,
		FADE_OUT,
		
		END
	};

	int mState = 0;

public:
	void Initialize() override;
	void Update() override;
};