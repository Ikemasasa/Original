#include "ItemSelect.h"

#include "lib/Audio.h"
#include "lib/ConvertString.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "Define.h"
#include "KeyGuide.h"
#include "Item.h"

void ItemSelect::Initialize()
{
	mSelectIndex = 0;

	mItemBoard = std::make_unique<Sprite>(L"Data/Image/Menu/item_board.png");
	mSelectFrame = std::make_unique<Sprite>(L"Data/Image/Menu/item_board_select.png");
	mInfoBoard = std::make_unique<Sprite>(L"Data/Image/Menu/item_info_board.png");
}

void ItemSelect::Update(const Item* inventory)
{
	// SelectIndex操作
	mInventory = inventory;
	int inventorySize = mInventory->GetItemNum();
	int oldSelectIndex = mSelectIndex;

	// カーソル計算
	const int INDEX_MAX = inventorySize - 1;
	if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) ++mSelectIndex;
	if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  --mSelectIndex;
	if (mSelectIndex >= inventorySize) mSelectIndex = 0;
	if (mSelectIndex < 0) mSelectIndex = INDEX_MAX;

	if (Input::GetButtonTrigger(0, Input::BUTTON::UP))    mSelectIndex -= HORIZONTAL_NUM;
	if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN))  mSelectIndex += HORIZONTAL_NUM;
	if (mSelectIndex >= inventorySize)
	{
		int current = mSelectIndex / HORIZONTAL_NUM;
		int max = INDEX_MAX / HORIZONTAL_NUM;
		if (current == max) mSelectIndex = INDEX_MAX;
		else mSelectIndex = mSelectIndex % HORIZONTAL_NUM;
	}
	if (mSelectIndex < 0) mSelectIndex = Math::Min((INDEX_MAX) / HORIZONTAL_NUM * HORIZONTAL_NUM + (mSelectIndex + HORIZONTAL_NUM), INDEX_MAX);

	// キーガイド
	KeyGuide::Instance().Add(KeyGuide::DPAD, L"カーソル移動");

	// アイテム情報更新
	UpdateInfo();

	// サウンド
	if (oldSelectIndex != mSelectIndex) Audio::SoundPlay((int)Sound::CURSOR_MOVE);
}

void ItemSelect::Render(const Vector2& boardOffset)
{
	if (mSelectIndex >= mInventory->GetItemNum()) mSelectIndex = mInventory->GetItemNum() - 1;

	// ボード描画
	mItemBoard->Render(boardOffset, Vector2::ONE, Vector2::ZERO, mItemBoard->GetSize());



	// アイテムアイコン描画
	{
		// 選択したインベントリのアイテムを全部描画
		const float ICON_SCALE_SIZE = ICON_SCALE * ICON_SIZE;
		const Vector2 offset(ICON_OFFSET + boardOffset.x, ICON_OFFSET + boardOffset.y);
		const Vector2 scale(ICON_SCALE, ICON_SCALE);
		const Vector2 size(ICON_SIZE, ICON_SIZE);

		int itemNum = mInventory->GetItemNum();
		if (itemNum > 0)
		{
			for (int i = 0; i < itemNum; ++i)
			{
				float x = i % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x;
				float y = i / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y;
				Vector2 pos(x, y);
				mInventory->GetItemParam(i)->icon->Render(pos, scale, Vector2::ZERO, size);
			}

			// 選択のフレーム画像を描画
			Vector2 pos(mSelectIndex % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x, mSelectIndex / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y);
			mSelectFrame->Render(pos, scale, Vector2::ZERO, size);
		}
	}

	// アイテム情報描画
	{
		const Vector2 texPos(0.0f, 0.0f);

		// ボード
		const Vector2 infoBoardPos(boardOffset.x + INFO_BOARD_OFFSET_X, boardOffset.y);
		const Vector2 scale(1.0f, 1.0f);
		mInfoBoard->Render(infoBoardPos, scale, texPos, mInfoBoard->GetSize());

		// アイテムアイコン
		const Vector2 iconPos(infoBoardPos.x + INFO_ICON_OFFSET_X, infoBoardPos.y + INFO_ICON_OFFSET_Y);
		const Vector2 iconScale(INFO_ICON_SCALE, INFO_ICON_SCALE);
		const Vector2 iconSize(ICON_SIZE, ICON_SIZE);
		const ItemData::BaseData* base = mInventory->GetItemParam(mSelectIndex);
		if (base) base->icon->Render(iconPos, iconScale, texPos, iconSize);

		// 説明
		RenderSetInfo(infoBoardPos);
		mFont.Render();
	}

}

void ItemSelect::UpdateInfo()
{
	// インフォ更新
	// 前回のアイテム情報を削除
	mInfo.clear();

	const ItemData::BaseData* base = mInventory->GetItemParam(mSelectIndex);
	if (!base)
	{
		mInfo.push_back(L"アイテムを持ってません");
		return;
	}

	// 名前
	mInfo.push_back(base->name.c_str());

	// 情報
	mInfo.push_back(base->info);
}

void ItemSelect::RenderSetInfo(const Vector2& infoBoardPos)
{
	Vector2 pos;// あとでボードの座標を足す(相対座標
	Vector2 center(0.0f, 0.0f);
	Vector2 scale(1.0f, 1.0f);
	
	const float NAME_OFFSET_Y = 28;
	for (size_t i = 0; i < mInfo.size(); ++i)
	{
		// アイテム名か、アイテムの情報かで分ける
		if (i == 0)
		{
			// 名前
			pos = infoBoardPos + Vector2(mInfoBoard->GetSize().x / 2.0f, NAME_OFFSET_Y);
			center = Vector2(0.5f, 0.0f);
			mFont.RenderSet(mInfo[i].c_str(), pos, center, Define::FONT_COLOR);
		}
		else
		{
			// 情報

			// 数行に分ける
			const wchar_t* text = mInfo[i].c_str();
			size_t size = mInfo[i].size();
			std::wstring str[INFO_LINE_MAX];
			int strIndex = 0;
			for (size_t i = 0; i < size; ++i)
			{
				str[strIndex] += text[i];
				if (mFont.GetWidth(str[strIndex].c_str()) >= INFO_WIDTH_MAX)
				{
					strIndex = Math::Min(strIndex + 1, INFO_LINE_MAX - 1);
				}
			}

			// レンダーセット
			pos = infoBoardPos + Vector2(INFO_TEXT_OFFSET_X, INFO_TEXT_OFFSET_Y);
			center = Vector2(0.0f, 0.0f);
			for (int i = 0; i < INFO_LINE_MAX; ++i)
			{
				pos.y += INFO_TEXT_ADD_Y * i;
				mFont.RenderSet(str[i].c_str(), pos, center, Define::FONT_COLOR);
			}
		}
	}
}

Vector2 ItemSelect::GetCursorRightUpPos() const
{
	const float ICON_SCALE_SIZE = ICON_SIZE * ICON_SCALE;
	float x = ICON_OFFSET + mSelectIndex % HORIZONTAL_NUM * ICON_SCALE_SIZE + ICON_SCALE_SIZE;
	float y = ICON_OFFSET + mSelectIndex / HORIZONTAL_NUM * ICON_SCALE_SIZE;
	return Vector2(x, y);
}
