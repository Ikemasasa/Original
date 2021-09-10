#include "IDecideCommand.h"

#include "lib/Audio.h"
#include "lib/Input.h"

void IDecideCommand::SoundPlay()
{
	if (Input::GetButtonTrigger(0, Input::BUTTON::A)) Audio::SoundPlay((int)Sound::SELECT);
	if (Input::GetButtonTrigger(0, Input::BUTTON::B)) Audio::SoundPlay((int)Sound::CANCEL);
}
