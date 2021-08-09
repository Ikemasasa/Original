#include "CommandItem.h"

#include "lib/Input.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "CommandCharaSelect.h"
#include "Define.h"
#include "Item.h"


CommandItem::CommandItem()
{
	mItemSelect.Initialize();
}

CommandItem::~CommandItem()
{
}

void CommandItem::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	BattleCharacter* moveChara = bcm->GetMoveChara();

	int itemIndex = mItemSelect.Update(moveChara->GetInventory());
	const int noSelect = -1;
	if (noSelect != itemIndex)
	{
		// アイテム選択したら
		
		// アイテム情報取得
		cmdBase->SetItemIndex(itemIndex);
		const ItemData::ItemParam* param = moveChara->GetInventory()->GetItemParam(itemIndex);

		// アイテムの対象によってどっちのきゃらを選ぶか決める
		Character::Type charaType;
		if (param->target == ItemData::Target::PARTY) charaType = Character::PLAYER;
		else if (param->target == ItemData::Target::ENEMY) charaType = Character::ENEMY;
		
		// 次のコマンド
		mNextCommand = std::make_unique<CommandCharaSelect>(charaType);
	}

	// CommandBehaviourに戻る
	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		cmdBase->SetItemIndex(noSelect);
		mIsBackState = true;
	}
}

void CommandItem::Render()
{
	const Vector2 boardOffset(Define::SCREEN_WIDTH / 10.0f, Define::SCREEN_HEIGHT / 5.0f);
	mItemSelect.Render(boardOffset);
}
