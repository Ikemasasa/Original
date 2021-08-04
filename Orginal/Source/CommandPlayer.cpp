#include "CommandPlayer.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CameraManager.h"
#include "CommandBehaviour.h"
#include "CommandItem.h"

#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

CommandPlayer::CommandPlayer() : CommandBase()
{
	mCommand.push(std::make_unique<CommandBehaviour>());
}

CommandPlayer::~CommandPlayer()
{
	size_t size = mCommand.size();
	for (size_t i = 0; i < size; ++i) mCommand.pop();
}

void CommandPlayer::Update(const BattleCharacterManager* bcm)
{
	if (IsBehaviourEnable()) return;

	// コマンドを1つ戻す
	if (mCommand.top()->IsBackState())
	{
		mCommand.pop();
	}

	// 次のコマンドがあるなら更新する
	IDecideCommand* nextCommand = mCommand.top()->GetNextCommandState();
	if (nextCommand)
	{
		mCommand.emplace(nextCommand);
	}

	mCommand.top()->Update(bcm, this);
	if (IsBehaviourEnable()) // 1行上のupdateでコマンドが決まったら
	{
		// コマンドをリセットする(CommandBehaviourだけにする)
		size_t popNum = mCommand.size() - 1; // CommandBehaviourだけ残したいから size - 1
		for (size_t i = 0; i < popNum; ++i) mCommand.pop();
	}
}

void CommandPlayer::Render() const
{
	if (IsBehaviourEnable()) return;

	mCommand.top()->Render();
}
