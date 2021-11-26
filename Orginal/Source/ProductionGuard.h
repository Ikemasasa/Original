#pragma once
#include "IBattleProduction.h"

#include "lib/Font.h"

class ProductionGuard : public IBattleProduction
{
	enum State
	{
		INIT,
		WAIT,

		END
	};
	State mState;
	Font mFont;

public:
	ProductionGuard() = default;
	~ProductionGuard() = default;

	void Initialize() override;
	void Update() override;
	void Render() override;
};