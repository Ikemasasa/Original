#include "CommandEscape.h"

#include "lib/Input.h"

#include "CommandBase.h"

CommandEscape::CommandEscape()
{
	// 画像読み込み(都合よさそうだから、health_plateを使ってる)
	mBoard = std::make_unique<Sprite>(L"Data/Image/Menu/health_plate.png");

	// フォントセット
	{
		mFont.Initialize();
		mFont.Add(L"逃げますか？");
		mFont.Add(L"A : はい");
		mFont.Add(L"B : いいえ");

		const Vector4 colors[] =
		{
			{0.56f, 0.3f, 0.2f, 1.0f},
			{0.93f, 0.25f, 0.13f, 1.0f},
			{0.0f, 0.32f, 0.65f, 1.0f}
		};

		Vector2 pos(PLATE_POS_X, PLATE_POS_Y + FIRST_OFFSET_Y - mBoard->GetSize().y/ 2.0f);
		const Vector2 SCALE = Vector2::ONE;
		for (int i = 0; i < mFont.GetNum(); ++i)
		{
			Vector2 center(mFont.GetWidth(i) / 2.0f, 0.0f);
			mFont.RenderSet(i, pos, center, SCALE, colors[i]);

			pos.y += ADD_OFFSET_Y;
		}
	}

}

CommandEscape::~CommandEscape()
{
	mFont.ClearRenderSet();
}

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

void CommandEscape::Render()
{
	Vector2 pos(PLATE_POS_X, PLATE_POS_Y);
	Vector2 scale = Vector2::ONE;
	Vector2 texPos = Vector2::ZERO;
	Vector2 center = mBoard->GetSize() / 2.0f;
	mBoard->Render(pos, scale, texPos, mBoard->GetSize(), center);
	mFont.Render(false);
}
