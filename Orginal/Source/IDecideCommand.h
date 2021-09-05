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
	int mSelectIndex = 0;
	bool mIsBackState = false;

public:
	IDecideCommand() = default;
	virtual ~IDecideCommand() = default;

	virtual void Initialize(const BattleCharacterManager* bcm) = 0;
	virtual void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) = 0;
	virtual void Render() {}

	virtual void ResetParam() {}
	void SoundPlay();

	bool IsBackState() const { return mIsBackState; }
	int GetSelectIndex() const { return mSelectIndex; }
	IDecideCommand* GetNextCommandState() { return mNextCommand.release(); }
};