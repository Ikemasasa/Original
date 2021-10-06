#include "MenuItem.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "Item.h"
#include "ItemData.h"
#include "KeyGuide.h"
#include "Player.h"
#include "PlayerManager.h"
#include "StatusData.h"

void MenuItem::Initialize(const PlayerManager* plm)
{
	mCharacterHealth.Initialize(Vector2(HEALTH_BOARD_X, HEALTH_BOARD_Y));
	mCharacterSelect.Initialize(plm);
	mItemSelect.Initialize();
	mSelectOptions.Initialize();
	mItemIndex = -1;
}

MenuBase::Select MenuItem::Update(PlayerManager* plm)
{
	if (mItemIndex == -1)
	{
		mCharacterSelect.Update(); // 誰のインベントリを参照するか決める

		mInventory = plm->GetPlayer(mCharacterSelect.GetIndex())->GetInventory(); // 参照するインベントリ保存
		mItemSelect.Update(mInventory); 

		// アイテム選択
		if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		{
			int index = mItemSelect.GetIndex();
			const ItemData::BaseData* base = mInventory->GetItemParam(index);
			
			// 回復アイテムか、フィールド使用系アイテムなら使える
			if (base->type == ItemData::HEAL || base->type == ItemData::FIELD_USE)
			{
				mItemIndex = mItemSelect.GetIndex();
			}
			else
			{
				Audio::SoundStop((int)Sound::SELECT);
				Audio::SoundPlay((int)Sound::CANCEL);
			}
		}

		// 前の画面に戻る
		if (Input::GetButtonTrigger(0, Input::BUTTON::B)) return Select::BACK;

		KeyGuide::Instance().Add(KeyGuide::A, L"決定");
		KeyGuide::Instance().Add(KeyGuide::B, L"戻る");
	}
	else
	{
		// アイテム情報を取得
		const ItemData::BaseData* baseData = mInventory->GetItemParam(mItemIndex);

		switch (baseData->type)
		{
		case ItemData::HEAL:	  UseHealItem(plm, baseData->id); break;
		case ItemData::FIELD_USE: UseFieldUseItem(baseData->id); break;
		}

		if (Input::GetButtonTrigger(0, Input::BUTTON::B) && !mIsUseFieldItem)
		{
			mItemIndex = -1; // アイテム未選択状態に戻す
		}

		// キーガイド
		KeyGuide::Instance().Add(KeyGuide::B, L"戻る");
	}

	return MenuBase::NONE;
}

void MenuItem::Render()
{
	mCharacterSelect.Render(Vector2(BOARD_OFFSET_X + ItemSelect::ICON_OFFSET, BOARD_OFFSET_Y));
	mItemSelect.Render(Vector2(BOARD_OFFSET_X, BOARD_OFFSET_Y));

	if (mItemIndex != -1)
	{
		// アイテム情報取得
		const ItemData::BaseData* baseData = mInventory->GetItemParam(mItemIndex);

		if (baseData->type == ItemData::HEAL)	   mCharacterHealth.Render(true);
		if (baseData->type == ItemData::FIELD_USE)
		{
			mSelectOptions.Render(Vector2(BOARD_OFFSET_X, BOARD_OFFSET_Y) + mItemSelect.GetCursorRightUpPos());
		}
	}
} 

void MenuItem::Release()
{

}

void MenuItem::UseFieldUseItem(const int itemID)
{
	// 選択肢作成
	mSelectOptions.AddTitle(L"使用しますか？");
	mSelectOptions.AddOption(L"はい");
	mSelectOptions.AddOption(L"いいえ");
	mSelectOptions.Update();

	// すでに使用中なら
	if (mIsUseFieldItem)
	{
		// アイテム情報を取得
		const ItemData::BaseData* baseData = mInventory->GetItemParam(mItemIndex);
		const FieldUseItemData::Param param = FieldUseItemData::GetParam(baseData->id);
		param.effect->Execute();

		if (param.effect->IsEffectFinished())
		{
			// アイテム減らす
			mInventory->Sub(baseData->id);

			// アイテム未選択状態に戻す
			mItemIndex = -1;
		}

		return;
	}

	// 選択
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		switch (mSelectOptions.GetIndex())
		{
		case 0: mIsUseFieldItem = true; break;
		case 1: mItemIndex = -1; break;
		}
	}
}

void MenuItem::UseHealItem(const PlayerManager* plm, const int itemID)
{
	// ステータス配列作成
	int statusNum = plm->GetNum();
	std::vector<Status> statusArray;
	for (int i = 0; i < statusNum; ++i)
	{
		int charaID = plm->GetPlayer(i)->GetCharaID();
		statusArray.push_back(StatusData::GetPLStatus(charaID));
	}
	mCharacterHealth.Update(statusArray);

	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		// 回復アイテムの情報を取得
		UseItemData::Param param = UseItemData::GetParam(itemID);

		// ステータス取得
		int charaindex = mCharacterHealth.GetSelectIndex();
		int charaID = plm->GetPlayer(charaindex)->GetCharaID();
		Status plStatus = StatusData::GetPLStatus(charaID);

		// 回復アイテムが使えるかチェックする
		bool useable = false;
		if (!plStatus.IsFullHP() && param.hpValue > 0) useable = true; // HPがmaxじゃないかつhp回復
		if (!plStatus.IsFullMP() && param.mpValue > 0) useable = true; // MPがmaxじゃないかつmp回復
		if (!useable) // 使えないならreturn
		{
			Audio::SoundStop((int)Sound::SELECT);
			Audio::SoundPlay((int)Sound::CANCEL);
			return;
		}

		// 回復
		plStatus.AddHP(param.hpValue);
		plStatus.AddMP(param.mpValue);
		Audio::SoundPlay((int)Sound::HEAL);

		// ステータス更新
		StatusData::SetPLStatus(charaID, plStatus);

		// アイテム減らす
		mInventory->Sub(param.base->id);

		// アイテム未選択状態に戻す
		mItemIndex = -1;
	}
}
