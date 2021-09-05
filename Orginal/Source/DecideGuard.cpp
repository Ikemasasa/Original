#include "DecideGuard.h"

#include "lib/Input.h"

#include "CommandBase.h"

void DecideGuard::Initialize(const BattleCharacterManager* bcm)
{
	// 選択肢セット
	mSelectOptions.Initialize();
	mSelectOptions.AddTitle(L"ガードしますか？");
	mSelectOptions.AddOption(L"ガードする");
	mSelectOptions.AddOption(L"ガードしない");
}

void DecideGuard::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	// 効果音を鳴らす
	SoundPlay();
	mSelectOptions.Update();

	// 選択肢で分岐
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		int index = mSelectOptions.GetIndex();
		switch (index)
		{
		case 0: cmdBase->SetBehaviour(CommandBase::Behaviour::GUARD); break;
		case 1: mIsBackState = true; break;
		}
	}

	// キャンセルボタンでも戻れるようにする
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
