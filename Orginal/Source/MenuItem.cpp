#include "MenuItem.h"

#include <map>

#include "lib/ConvertString.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Texture.h"

#include "Item.h"
#include "ItemData.h"
#include "MenuCharacterSelect.h"
#include "PlayerManager.h"
#include "Singleton.h"

MenuItem::MenuItem(PlayerManager* plm)
{
	mPlayerManager = plm;

	mCharacterSelect = std::make_unique<MenuCharacterSelect>(mPlayerManager);
	mSelect = std::make_unique<Texture>(L"Data/Image/select_frame.png");
	mBoard = std::make_unique<Texture>(L"Data/Image/board.png");

	mFont.Initialize(64, 64);
}

MenuItem::~MenuItem()
{
	mFont.Release();
}

MenuBase::Select MenuItem::Update(PlayerManager* plm)
{
	switch (mState)
	{
	case INVENTORY_SELECT: InventorySelect(); break;
	case ITEM_SELECT: ItemSelect(); break;
	case TARGET_SELECT: TargetSelect(); break;
	}




	return MenuBase::NONE;
}

void MenuItem::Render()
{
	RenderItem();

	mFont.Render();
} 

void MenuItem::Release()
{

}

void MenuItem::RenderItem()
{
	// ボード描画
	mBoard->Render(Vector2(BOARD_OFFSET_X, BOARD_OFFSET_Y), Vector2::One(), Vector2::Zero(), mBoard->GetSize());


	// アイテムアイコン描画
	const float ICON_SCALE_SIZE = ICON_SCALE * ICON_SIZE;
	const Vector2 offset(ICON_OFFSET + BOARD_OFFSET_X, ICON_OFFSET + BOARD_OFFSET_Y);
	const Vector2 scale(ICON_SCALE, ICON_SCALE);
	const Vector2 size(ICON_SIZE, ICON_SIZE);

	const std::vector<std::shared_ptr<Player>>& players = mPlayerManager->GetPlayers();
	int itemNum = 0;
	for (auto& pl : players)
	{
		const std::vector<ItemData::ItemParam>& inventory = pl->GetInventory()->GetAll();
		for (auto& item : inventory)
		{
			float x = itemNum % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x;
			float y = itemNum / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y;
			Vector2 pos(x, y);
			item.icon->Render(pos, scale, Vector2::Zero(), size);

			++itemNum;
		}
	}

	// 選択のフレーム画像を描画
	{
		Vector2 pos(mSelectIndex % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x, mSelectIndex / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y);
		mSelect->Render(pos, scale, Vector2::Zero(), size);
	}
}

void MenuItem::InventorySelect()
{
	// キャラクター選択のインデックス操作
	mCharacterSelect->Update();

	// キャラ選択
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		int index = mCharacterSelect->GetSelectIndex();
		mSelectInventory = mPlayerManager->GetPlayers()[index]->GetInventory();
		mState = ITEM_SELECT;
	}
}

void MenuItem::ItemSelect()
{
	// SelectIndex操作
	{
		if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) ++mSelectIndex;
		if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  --mSelectIndex;
		if (Input::GetButtonTrigger(0, Input::BUTTON::UP))    mSelectIndex -= HORIZONTAL_NUM;
		if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN))  mSelectIndex += HORIZONTAL_NUM;
		mSelectIndex = Math::Clamp(mSelectIndex, 0, mSelectInventory->GetAll().size());
	}


	// アイテム選択
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		ItemData::Effect effect = mSelectInventory->GetAll()[mSelectIndex].effect;
		if (effect == ItemData::Effect::HEAL) // 回復アイテムなら使用可能
		{
			mCharacterSelect->ResetSelectIndex(); // キャラ選択のやつを使いまわすため
			mState = TARGET_SELECT;
		}
	}
}

void MenuItem::TargetSelect()
{
	// キャラクター選択のインデックス操作
	mCharacterSelect->Update();

	// 選択
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		int charaindex = mCharacterSelect->GetSelectIndex();

		mSelectInventory->GetAll()[mSelectIndex].hpValue;

	}
}