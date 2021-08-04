#include "CommandBehaviour.h"

#include "lib/Input.h"
#include "lib/Math.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CommandItem.h"
#include "CommandCharaSelect.h"

CommandBehaviour::CommandBehaviour()
{
	mIcons = std::make_unique<Sprite>(L"Data/Image/Battle/command_icons.png");
	mCommandIndex = { 1, 1 };
}

void CommandBehaviour::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	BattleState::GetInstance().SetState(BattleState::State::COMMAND_SELECT);

	// もっと最適化できると思うけど、とりあえずゴリ
	mOldCommandIndex = mCommandIndex;

	if (Input::GetButtonTrigger(0, Input::UP))    mCommandIndex.y = Math::Max(mCommandIndex.y - 1, COMMAND_MIN_Y);
	if (Input::GetButtonTrigger(0, Input::DOWN))  mCommandIndex.y = Math::Min(mCommandIndex.y + 1, COMMAND_MAX_Y);
	if (Input::GetButtonTrigger(0, Input::RIGHT)) mCommandIndex.x = Math::Min(mCommandIndex.x + 1, COMMAND_MAX_X);
	if (Input::GetButtonTrigger(0, Input::LEFT))  mCommandIndex.x = Math::Max(mCommandIndex.x - 1, COMMAND_MIN_X);


	if (mCommandIndex.y != 1 && mOldCommandIndex.y != mCommandIndex.y)	// Yが1以外で 前回のYと違う値なら
	{
		mCommandIndex.x = 1;
	}
	if (mCommandIndex.x != 1 && mOldCommandIndex.x != mCommandIndex.x) // Xが1以外で 前回のXと違う値なら
	{
		mCommandIndex.y = 1;
	}


	// こんなレイアウト
	//    道
	// 防 攻 技
	//    逃
	if (Input::GetButtonTrigger(0, Input::A))
	{
		if (mCommandIndex.y == 0) mNextCommand = std::make_unique<CommandItem>();
		else if (mCommandIndex.y == 2);
		else if (mCommandIndex.x == 0);
		else if (mCommandIndex.x == 1) mNextCommand = std::make_unique<CommandCharaSelect>(Character::Type::ENEMY);
		else if (mCommandIndex.x == 2);
	}
}

void CommandBehaviour::Render()
{
	const float offsetX = 300.0f;
	const float offsetY = 300.0f;

	const Vector2 scale(ICON_SCALE, ICON_SCALE);
	const Vector2 size(ICON_SIZE, ICON_SIZE);
	const Vector2 scaleSize = Vector2(size.x * scale.x, size.y * scale.y);

	// 上から「道具」「防御」「攻撃」「技」「逃」「選択中」の画像
	mIcons->Render(Vector2(offsetX + scaleSize.x, offsetY)													, scale, Vector2(ICON_ITEM_X, 0.0f)	  , size);
	mIcons->Render(Vector2(offsetX, offsetY + scaleSize.y)													, scale, Vector2(ICON_DEFENCE_X, 0.0f), size);
	mIcons->Render(Vector2(offsetX + scaleSize.x, offsetY + scaleSize.y)									, scale, Vector2(ICON_ATTACK_X, 0.0f) , size);
	mIcons->Render(Vector2(offsetX + scaleSize.x * 2, offsetY + scaleSize.y)								, scale, Vector2(ICON_SKILL_X, 0.0f)  , size);
	mIcons->Render(Vector2(offsetX + scaleSize.x, offsetY + scaleSize.y * 2)								, scale, Vector2(ICON_ESCAPE_X, 0.0f) , size);
	mIcons->Render(Vector2(offsetX + mCommandIndex.x * scaleSize.x, offsetY + mCommandIndex.y * scaleSize.y), scale, Vector2(ICON_SELECT_X, 0.0f) , size);
}
