#pragma once
#include <memory>

#include "Define.h"
#include "IDecideCommand.h"
#include "SelectOptions.h"

class DecideEscape : public IDecideCommand
{
	static constexpr float BOARD_POS_X = 200.0f;
	static constexpr float BOARD_POS_Y = 220.0f;

	SelectOptions mSelectOptions;

public:
	DecideEscape() = default;
	~DecideEscape() = default;

	void Initialize(const BattleCharacterManager* bcm) override;
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	void Render();
};