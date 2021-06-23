#pragma once
#include <memory>

class BattleActor;
class BattleActorManager;
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

	virtual void Update(const BattleActorManager* bam, CommandBase* cmdBase) = 0;
	virtual void Render() const {}

	bool IsBackState() const { return mIsBackState; }

	IDecideCommand* ReleaseNextCommandState() { return mNextCommand.release(); }
};