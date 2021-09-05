#include "DecideEscape.h"

#include "lib/Input.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "Define.h"

void DecideEscape::Initialize(const BattleCharacterManager* bcm)
{
	// �I�����Z�b�g
	mSelectOptions.Initialize();
	mSelectOptions.AddTitle(L"�����܂����H");
	mSelectOptions.AddOption(L"������");
	mSelectOptions.AddOption(L"�����Ȃ�");
}

void DecideEscape::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
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
		case 0: cmdBase->SetBehaviour(CommandBase::Behaviour::ESCAPE); break;
		case 1: mIsBackState = true; break;
		}
	}

	// �L�����Z���{�^���ł��߂��悤�ɂ���
	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
	}
}

void DecideEscape::Render()
{
	Vector2 pos(BOARD_POS_X, BOARD_POS_Y);
	mSelectOptions.Render(pos, false);
}
