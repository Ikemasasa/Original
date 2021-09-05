#pragma once
#include "IDecideCommand.h"

#include "SelectOptions.h"

class DecideGuard : public IDecideCommand
{
	static constexpr float BOARD_POS_X = 200.0f;
	static constexpr float BOARD_POS_Y = 220.0f;

	SelectOptions mSelectOptions;

public:
	void Initialize(const BattleCharacterManager* bcm) override;
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	void Render() override;
};