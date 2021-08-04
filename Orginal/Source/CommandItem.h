#pragma once
#include "IDecideCommand.h"

#include "ItemSelect.h"

class CommandItem : public IDecideCommand
{
	ItemSelect mItemSelect;

public:
	CommandItem();
	~CommandItem();
	virtual void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	virtual void Render() override;
};