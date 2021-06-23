#include "CommandItem.h"

#include "lib/Input.h"

#include "BattleActorManager.h"
#include "CommandItem.h"
#include "Define.h"
#include "Fade.h"
#include "Item.h"
#include "PlayerBattle.h"


CommandItem::CommandItem()
{
	mItemSelect.Initialize();
}

CommandItem::~CommandItem()
{
}

void CommandItem::Update(const BattleActorManager* bam, CommandBase* cmdBase)
{
	const BattleActor* moveActor = bam->GetMoveActor();

	int itemIndex = mItemSelect.Update(moveActor->GetInventory());
	const int noSelect = -1;
	if (noSelect != itemIndex)
	{
		// アイテム選択したら
		ItemData::ItemParam param = moveActor->GetInventory()->GetItemParam(itemIndex);
		cmdBase->SetItemParam(&param);

		if (param.target == ItemData::Target::PARTY);
		else if (param.target == ItemData::Target::ENEMY);
	}

	// CommandBehaviourに戻る
	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
	}
}

void CommandItem::Render() const
{
	const Vector2 boardOffset(Define::SCREEN_WIDTH / 10.0f, Define::SCREEN_HEIGHT / 5.0f);
	mItemSelect.Render(boardOffset);
}
