#include "CommandPlayer.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CameraManager.h"
#include "CommandBehaviour.h"
#include "CommandItemSelect.h"

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
		nextCommand->Initialize(bcm);
		mCommand.emplace(nextCommand);
	}

	int oldIndex = mCommand.top()->GetSelectIndex();
	mCommand.top()->Update(bcm, this);
	int currentIndex = mCommand.top()->GetSelectIndex();

	// 効果音をならす
	if (Input::GetButtonTrigger(0, Input::BUTTON::A)) AUDIO.SoundPlay((int)Sound::SELECT);
	if (Input::GetButtonTrigger(0, Input::BUTTON::B)) AUDIO.SoundPlay((int)Sound::CANCEL);
	if (oldIndex != currentIndex) AUDIO.SoundPlay((int)Sound::CURSOR_MOVE);

	if (IsBehaviourEnable()) // コマンドが決まったら
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

void CommandPlayer::BehaviourFinished()
{
	mCommand.top()->ResetParam();
	CommandBase::BehaviourFinished();
}
