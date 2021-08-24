#pragma once
#include "IDecideCommand.h"

#include <vector>

#include "SkillSelect.h"

class CommandSkillSelect : public IDecideCommand
{
	static constexpr float BOARD_POS_X = 300;
	static constexpr float BOARD_POS_Y = 200;

	SkillSelect mSkillSelect;

public:
	CommandSkillSelect() = default;
	~CommandSkillSelect() = default;

	void Initialize(const BattleCharacterManager* bcm) override;
	void Update(const BattleCharacterManager* bcm, CommandBase* cb) override;
	void Render() override;


};