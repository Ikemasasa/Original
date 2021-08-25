#include "DecideEscape.h"

#include "lib/Input.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "DataBase.h"
#include "Define.h"
#include "Singleton.h"
#include "StatusData.h"


void DecideEscape::Initialize(const BattleCharacterManager* bcm)
{
	// 画像読み込み(都合よさそうだから、health_boardを使ってる)
	mBoard = std::make_unique<Sprite>(L"Data/Image/Menu/health_board.png");

	// フォントセット
	{
		mFont.Initialize();
		mFont.Add(L"逃げますか？");
		mFont.Add(L"A : はい");
		mFont.Add(L"B : いいえ");

		Vector2 pos(BOARD_POS_X, BOARD_POS_Y + FIRST_OFFSET_Y - mBoard->GetSize().y / 2.0f);
		for (size_t i = 0; i < mFont.GetNum(); ++i)
		{
			Vector2 center(mFont.GetWidth(i) / 2.0f, 0.0f);
			mFont.RenderSet(i, pos, center, Define::FONT_COLOR);

			pos.y += ADD_OFFSET_Y;
		}
	}
}

void DecideEscape::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	// 逃げる
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		// ステータス保存
		for (const auto& chara : bcm->GetBCharacters())
		{
			if (chara->GetType() != Character::PLAYER) break;
			Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(chara->GetCharaID(), *chara->GetStatus());
		}

		cmdBase->SetBehaviour(CommandBase::Behaviour::ESCAPE);
	}
	else if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
	}
}

void DecideEscape::Render()
{
	Vector2 pos(BOARD_POS_X, BOARD_POS_Y);
	Vector2 scale = Vector2::ONE;
	Vector2 texPos = Vector2::ZERO;
	Vector2 center = mBoard->GetSize() / 2.0f;
	mBoard->Render(pos, scale, texPos, mBoard->GetSize(), center);
	mFont.Render(false);
}
