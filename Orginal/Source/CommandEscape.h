#pragma once

#include "IDecideCommand.h"

class CommandEscape : public IDecideCommand
{
public:
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
};