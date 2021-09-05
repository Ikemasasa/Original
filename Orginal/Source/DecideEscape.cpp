#include "DecideEscape.h"

#include "lib/Input.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "Define.h"

void DecideEscape::Initialize(const BattleCharacterManager* bcm)
{
	// 選択肢セット
	mSelectOptions.Initialize();
	mSelectOptions.AddTitle(L"逃げますか？");
	mSelectOptions.AddOption(L"逃げる");
	mSelectOptions.AddOption(L"逃げない");
}

void DecideEscape::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
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
		case 0: cmdBase->SetBehaviour(CommandBase::Behaviour::ESCAPE); break;
		case 1: mIsBackState = true; break;
		}
	}

	// キャンセルボタンでも戻れるようにする
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
