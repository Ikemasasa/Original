#pragma once
#include "CommandBase.h"

class CommandEnemy : public CommandBase
{
public:
	void Update(const BattleActorManager* bam) override;
};