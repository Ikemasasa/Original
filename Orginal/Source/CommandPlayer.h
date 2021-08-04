#pragma once
#include <memory>
#include <stack>

#include "CommandBase.h"

class IDecideCommand;
class Sprite;

class CommandPlayer : public CommandBase
{
private:
	std::stack<std::unique_ptr<IDecideCommand>> mCommand;

public:
	CommandPlayer();
	~CommandPlayer();
	void Update(const BattleCharacterManager* bcm) override;
	void Render() const override;
};