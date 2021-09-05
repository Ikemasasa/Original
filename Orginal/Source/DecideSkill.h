#pragma once
#include "IDecideCommand.h"

#include <vector>

#include "SkillSelect.h"

class DecideSkill : public IDecideCommand
{
	static constexpr float BOARD_POS_X = 300;
	static constexpr float BOARD_POS_Y = 200;

	SkillSelect mSkillSelect;

public:
	DecideSkill() = default;
	~DecideSkill() = default;

	void Initialize(const BattleCharacterManager* bcm) override;
	void Update(const BattleCharacterManager* bcm, CommandBase* cb) override;
	void Render() override;


};