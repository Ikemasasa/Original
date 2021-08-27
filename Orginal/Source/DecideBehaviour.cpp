#include "DecideBehaviour.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CommandBase.h"
#include "DecideTargetChara.h"
#include "DecideEscape.h"
#include "DecideItem.h"
#include "DecideSkill.h"

DecideBehaviour::DecideBehaviour()
{
	mIcons = std::make_unique<Sprite>(L"Data/Image/Battle/command_icons.png");
	mCommandNameBoard = std::make_unique<Sprite>(L"Data/Image/Battle/command_name_board.png");
	mCommandIndex = { 1, 1 };
}

void DecideBehaviour::Initialize(const BattleCharacterManager* bcm)
{

}

void DecideBehaviour::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	BattleState::GetInstance().SetState(BattleState::State::COMMAND_SELECT);

	// 効果音を鳴らす
	SoundPlay();

	// もっと最適化できると思うけど、とりあえずゴリ
	TVector2<int> oldIndex = mCommandIndex;
	if (Input::GetButtonTrigger(0, Input::UP))    mCommandIndex.y = Math::Max(mCommandIndex.y - 1, COMMAND_MIN_Y);
	if (Input::GetButtonTrigger(0, Input::DOWN))  mCommandIndex.y = Math::Min(mCommandIndex.y + 1, COMMAND_MAX_Y);
	if (Input::GetButtonTrigger(0, Input::RIGHT)) mCommandIndex.x = Math::Min(mCommandIndex.x + 1, COMMAND_MAX_X);
	if (Input::GetButtonTrigger(0, Input::LEFT))  mCommandIndex.x = Math::Max(mCommandIndex.x - 1, COMMAND_MIN_X);

	if (mCommandIndex.y != 1 && oldIndex.y != mCommandIndex.y)	// Yが1以外で 前回のYと違う値なら
	{
		mCommandIndex.x = 1;
	}
	if (mCommandIndex.x != 1 && oldIndex.x != mCommandIndex.x) // Xが1以外で 前回のXと違う値なら
	{
		mCommandIndex.y = 1;
	}

	// CommandPlayerで効果音を鳴らすため、mSelectIndexに変換する
	mSelectIndex = mCommandIndex.y * COMMAND_VERTICAL + mCommandIndex.x;

	// こんなレイアウト
	//    道
	// 防 攻 技
	//    逃
	if (Input::GetButtonTrigger(0, Input::A))
	{
		if (mCommandIndex.y == 0) mNextCommand = std::make_unique<DecideItem>();
		else if (mCommandIndex.y == 2) mNextCommand = std::make_unique<DecideEscape>();
		else if (mCommandIndex.x == 0) cmdBase->SetBehaviour(CommandBase::Behaviour::GUARD);
		else if (mCommandIndex.x == 1) mNextCommand = std::make_unique<DecideTargetChara>(Character::Type::ENEMY);
		else if (mCommandIndex.x == 2) mNextCommand = std::make_unique<DecideSkill>();
	}
}

void DecideBehaviour::Render()
{
	const float LeftTopX = 300.0f;
	const float LeftTopY = 350.0f;

	const Vector2 scale(ICON_SCALE, ICON_SCALE);
	const Vector2 size(ICON_SIZE, ICON_SIZE);
	const Vector2 scaleSize = Vector2(size.x * scale.x, size.y * scale.y);

	// 上から「道具」「防御」「攻撃」「技」「逃」の座標
	Vector2 pos[] =
	{
		{ LeftTopX + scaleSize.x, LeftTopY },
		{ LeftTopX, LeftTopY + scaleSize.y },
		{ LeftTopX + scaleSize.x, LeftTopY + scaleSize.y },
		{ LeftTopX + scaleSize.x * 2, LeftTopY + scaleSize.y },
		{ LeftTopX + scaleSize.x, LeftTopY + scaleSize.y * 2 },
	};
	Vector2 selectPos = Vector2(LeftTopX + mCommandIndex.x * scaleSize.x, LeftTopY + mCommandIndex.y * scaleSize.y);

	const wchar_t* commandName[] =
	{
		L"アイテム", L"防御", L"攻撃", L"スキル", L"逃げる"
	};

	// コマンドの名前を入れるプレートを描画
	Vector2 nameBoardPos(LeftTopX, LeftTopY - mCommandNameBoard->GetSize().y * scale.y);
	mCommandNameBoard->Render(nameBoardPos, scale, Vector2::ZERO, mCommandNameBoard->GetSize());

	for (int i = 0; i < COMMAND_NUM; ++i)
	{
		// まずベースのアイコンを描画する
		mIcons->Render(pos[i], scale, Vector2(ICON_BASE_X, 0.0f), size);

		if (pos[i] == selectPos)
		{
			// 選択中の画像を描画する
			mIcons->Render(selectPos, scale, Vector2(ICON_SELECT_X, 0.0f), size);
			
			float fontOffsetY = 3.0f;
			Vector2 center(mFont.GetWidth(commandName[i]) / 2.0f, 0.0f);
			mFont.RenderSet(commandName[i], nameBoardPos + Vector2(mCommandNameBoard->GetSize().x / 2.0f * scale.x, fontOffsetY), center, Define::FONT_COLOR);
		}

		// それぞれのアイコンを描画する
		Vector2 texPos(i * ICON_SIZE, 0.0f);
		mIcons->Render(pos[i], scale, texPos, size);
	}

	mFont.Render();
}

void DecideBehaviour::ResetParam()
{
	// ターン開始時中央をさしててほしいので 1
	mCommandIndex.x = 1;
	mCommandIndex.y = 1;
}
