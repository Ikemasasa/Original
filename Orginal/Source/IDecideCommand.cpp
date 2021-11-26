#include "IDecideCommand.h"

#include "lib/Input.h"

#include "Sound.h"

void IDecideCommand::SoundPlay()
{
	if (Input::GetButtonTrigger(0, Input::BUTTON::A)) Sound::Play(Sound::SELECT);
	if (Input::GetButtonTrigger(0, Input::BUTTON::B)) Sound::Play(Sound::CANCEL);
}
