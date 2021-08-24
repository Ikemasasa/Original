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

	// �R�}���h��1�߂�
	if (mCommand.top()->IsBackState())
	{
		mCommand.pop();
	}

	// ���̃R�}���h������Ȃ�X�V����
	IDecideCommand* nextCommand = mCommand.top()->GetNextCommandState();
	if (nextCommand)
	{
		nextCommand->Initialize(bcm);
		mCommand.emplace(nextCommand);
	}

	int oldIndex = mCommand.top()->GetSelectIndex();
	mCommand.top()->Update(bcm, this);
	int currentIndex = mCommand.top()->GetSelectIndex();

	// ���ʉ����Ȃ炷
	if (Input::GetButtonTrigger(0, Input::BUTTON::A)) AUDIO.SoundPlay((int)Sound::SELECT);
	if (Input::GetButtonTrigger(0, Input::BUTTON::B)) AUDIO.SoundPlay((int)Sound::CANCEL);
	if (oldIndex != currentIndex) AUDIO.SoundPlay((int)Sound::CURSOR_MOVE);

	if (IsBehaviourEnable()) // �R�}���h�����܂�����
	{
		// �R�}���h�����Z�b�g����(CommandBehaviour�����ɂ���)
		size_t popNum = mCommand.size() - 1; // CommandBehaviour�����c���������� size - 1
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
