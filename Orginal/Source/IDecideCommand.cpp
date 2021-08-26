#include "IDecideCommand.h"

#include "lib/Audio.h"
#include "lib/Input.h"

void IDecideCommand::SoundPlay()
{
	if (Input::GetButtonTrigger(0, Input::BUTTON::A)) AUDIO.SoundPlay((int)Sound::SELECT);
	if (Input::GetButtonTrigger(0, Input::BUTTON::B)) AUDIO.SoundPlay((int)Sound::CANCEL);
}
