#pragma once
#include "IBattleProduction.h"

#include "lib/Font.h"

class ProductionGuard : public IBattleProduction
{
	static constexpr float WAIT_SEC = 1.0f;

	enum State
	{
		INIT,
		WAIT,

		END
	};
	State mState;
	float mTimer;

	Font mFont;
public:
	ProductionGuard() = default;
	~ProductionGuard() = default;

	void Initialize() override;
	void Update(const BattleCharacterManager * bcm) override;
	void Render() override;
};