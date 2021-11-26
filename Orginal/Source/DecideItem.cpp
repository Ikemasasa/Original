#include "DecideItem.h"

#include "lib/Input.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "DecideTargetChara.h"
#include "Define.h"
#include "Item.h"
#include "KeyGuide.h"

void DecideItem::Initialize(const BattleCharacterManager* bcm)
{
	mItemSelect.Initialize();
}

void DecideItem::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	// 効果音を鳴らす
	SoundPlay();

	BattleCharacter* moveChara = bcm->GetMoveChara();
	mItemSelect.Update(moveChara->GetInventory());

	// アイテム選択したら
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		// アイテム基本情報を取得
		const ItemData::BaseData* base = moveChara->GetInventory()->GetItemParam(mItemSelect.GetIndex());

		// 回復、ダメージアイテムなら
		if (base->type == ItemData::HEAL || base->type == ItemData::DAMAGE)
		{
			// 使用アイテム情報を取得
			UseItemData::Param param = UseItemData::GetParam(base->id);
			cmdBase->SetItemParam(&param);

			// 次のコマンドを決める
			Character::Type charaType = Character::NONE;
			switch (param.target)
			{
			case UseItemData::Target::PARTY: charaType = Character::PLAYER; break;
			case UseItemData::Target::ENEMY: charaType = Character::ENEMY;  break;
			}
			mNextCommand = std::make_unique<DecideTargetChara>(charaType);
		}
	}

	// CommandBehaviourに戻る
	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		cmdBase->SetItemParam(nullptr);
		mIsBackState = true;
	}

	// キーガイド設定
	KeyGuide::Instance().Add(KeyGuide::A, L"決定");
	KeyGuide::Instance().Add(KeyGuide::B, L"戻る");
}

void DecideItem::Render()
{
	const Vector2 boardOffset(Define::SCREEN_WIDTH / 10.0f, Define::SCREEN_HEIGHT / 5.0f);
	mItemSelect.Render(boardOffset);
}
