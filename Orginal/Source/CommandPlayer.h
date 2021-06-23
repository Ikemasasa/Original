#pragma once
#include <memory>
#include <stack>

#include "CommandBase.h"
//#include "IDecideCommand.h"

class IDecideCommand;
class Sprite;

class CommandPlayer : public CommandBase
{
private:
	std::stack<std::unique_ptr<IDecideCommand>> mCommand;

public:
	CommandPlayer();
	~CommandPlayer();
	void Update(const BattleActorManager* bam) override;
	void Render() const override;

	void ResetCommand();
};