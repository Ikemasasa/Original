#include "CommandItemSelect.h"

#include "lib/Input.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "CommandCharaSelect.h"
#include "Define.h"
#include "Item.h"

void CommandItemSelect::Initialize(const BattleCharacterManager* bcm)
{
	mItemSelect.Initialize();
}

void CommandItemSelect::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	BattleCharacter* moveChara = bcm->GetMoveChara();
	mItemSelect.Update(moveChara->GetInventory());

	// アイテム選択したら
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		// アイテム情報取得
		const ItemData::ItemParam* param = moveChara->GetInventory()->GetItemParam(mItemSelect.GetIndex());
		cmdBase->SetItemParam(param);

		// 次のコマンドを決める
		Character::Type charaType = Character::NONE;
		switch (param->target)
		{
		case ItemData::Target::PARTY: charaType = Character::PLAYER; break;
		case ItemData::Target::ENEMY: charaType = Character::ENEMY;  break;
		}
		mNextCommand = std::make_unique<CommandCharaSelect>(charaType);
	}

	// CommandBehaviourに戻る
	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		cmdBase->SetItemParam(nullptr);
		mIsBackState = true;
	}
}

void CommandItemSelect::Render()
{
	const Vector2 boardOffset(Define::SCREEN_WIDTH / 10.0f, Define::SCREEN_HEIGHT / 5.0f);
	mItemSelect.Render(boardOffset);
}
