#include "DecideGuard.h"

#include "lib/Input.h"

#include "CommandBase.h"

void DecideGuard::Initialize(const BattleCharacterManager* bcm)
{
	// �I�����Z�b�g
	mSelectOptions.Initialize();
	mSelectOptions.AddTitle(L"�K�[�h���܂����H");
	mSelectOptions.AddOption(L"�K�[�h����");
	mSelectOptions.AddOption(L"�K�[�h���Ȃ�");
}

void DecideGuard::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	// ���ʉ���炷
	SoundPlay();
	mSelectOptions.Update();

	// �I�����ŕ���
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		int index = mSelectOptions.GetIndex();
		switch (index)
		{
		case 0: cmdBase->SetBehaviour(CommandBase::Behaviour::GUARD); break;
		case 1: mIsBackState = true; break;
		}
	}

	// �L�����Z���{�^���ł��߂��悤�ɂ���
	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
	}
}

void DecideGuard::Render()
{
	Vector2 pos(BOARD_POS_X, BOARD_POS_Y);
	mSelectOptions.Render(pos, false);
}
