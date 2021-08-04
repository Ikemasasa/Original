#include "CommandEscape.h"

#include "lib/Input.h"

#include "CommandBase.h"

void CommandEscape::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		cmdBase->SetBehaviour(CommandBase::Behaviour::ESCAPE);
	}
	else if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
	}
}
