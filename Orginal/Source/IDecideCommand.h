#pragma once
#include <memory>

class BattleCharacter;
class BattleCharacterManager;
class CommandBase;


class IDecideCommand
{
public:
	enum class CmdState
	{
		ITEM,
		SELECT_PARTY,
		SELECT_ENEMY,

		MAX,
		BACK,
		NONE,
	};

protected:
	std::unique_ptr<IDecideCommand> mNextCommand;
	bool mIsBackState = false;

public:
	IDecideCommand() = default;
	virtual ~IDecideCommand() = default;

	virtual void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) = 0;
	virtual void Render() {}

	bool IsBackState() const { return mIsBackState; }

	IDecideCommand* GetNextCommandState() { return mNextCommand.release(); }
};