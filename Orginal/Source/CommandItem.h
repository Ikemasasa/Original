#pragma once
#include "IDecideCommand.h"

#include "ItemSelect.h"

class PlayerBattle;

class CommandItem : public IDecideCommand
{
	ItemSelect mItemSelect;

public:
	CommandItem();
	~CommandItem();
	virtual void Update(const BattleActorManager* bam, CommandBase* cmdBase) override;
	virtual void Render() const override;
};