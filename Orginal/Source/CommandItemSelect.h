#pragma once
#include "IDecideCommand.h"

#include "ItemSelect.h"

class CommandItemSelect : public IDecideCommand
{
	ItemSelect mItemSelect;

public:
	CommandItemSelect() = default;
	~CommandItemSelect() = default;

	void Initialize(const BattleCharacterManager* bcm) override;
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	void Render() override;
};