#include "EquipmentSelect.h"

#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "Define.h"
#include "KeyGuide.h"
#include "Sound.h"

void EquipmentSelect::Initialize()
{
	// 画像読み込み
	mBoard = std::make_unique<Sprite>(L"Data/Image/Menu/item_board.png");
	mSelectFrame = std::make_unique<Sprite>(L"Data/Image/Menu/item_board_select.png");
	mInfoBoard = std::make_unique<Sprite>(L"Data/Image/Menu/item_info_board.png");
	mEquipmentMark = std::make_unique<Sprite>(L"Data/Image/Menu/equipment_mark.png");
}

int EquipmentSelect::Update(const std::vector<EquipmentInventory::Data>& equipmentsData)
{
	// 引数から装備品リストを更新
	mEquipmentsData = equipmentsData;

	// 装備品の数を取得
	int inventorySize = mEquipmentsData.size();

	// 選択場所の操作
	int oldIndex = mSelectIndex;
	if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) ++mSelectIndex;
	if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  --mSelectIndex;
	if (mSelectIndex >= inventorySize) mSelectIndex -= inventorySize;
	if (mSelectIndex < 0) mSelectIndex += inventorySize;

	if (Input::GetButtonTrigger(0, Input::BUTTON::UP))    mSelectIndex -= HORIZONTAL_NUM;
	if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN))  mSelectIndex += HORIZONTAL_NUM;
	if (mSelectIndex >= inventorySize) mSelectIndex -= inventorySize + 1;
	if (mSelectIndex < 0) mSelectIndex += inventorySize + 1;
	mSelectIndex = Math::Clamp(mSelectIndex, 0, inventorySize - 1); // 帳尻あわんかったからclampしてる

	// 選択場所が変わったとき効果音を鳴らす
	if (oldIndex != mSelectIndex) Sound::Play(Sound::CURSOR_MOVE);

	
	// 装備品を決定したら、決定したときの選択場所を返す
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		return mSelectIndex;
	}

	// キーガイドを設定
	KeyGuide::Instance().Add(KeyGuide::DPAD, L"カーソル移動");
	KeyGuide::Instance().Add(KeyGuide::A, L"決定");

	return -1;
}

void EquipmentSelect::Render(const Vector2& boardPos)
{
	// ボード描画
	mBoard->Render(boardPos, Vector2::ONE, Vector2::ZERO, mBoard->GetSize());

	// 装備品の数が0 ならreturn 
	if (mEquipmentsData.empty()) return;

	//アイコン描画
	{
		const float ICON_SCALE_SIZE = ICON_SCALE * ICON_SIZE;
		const Vector2 offset(ICON_OFFSET + boardPos.x, ICON_OFFSET + boardPos.y);
		const Vector2 scale(ICON_SCALE, ICON_SCALE);
		const Vector2 size(ICON_SIZE, ICON_SIZE);

		int num = mEquipmentsData.size();
		for (int i = 0; i < num; ++i)
		{
			float x = i % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x;
			float y = i / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y;
			Vector2 pos(x, y);

			const EquipmentData::Param param = EquipmentData::GetParam(mEquipmentsData[i].equipmentID);
			param.base->icon->Render(pos, scale, Vector2::ZERO, size);

			// 装備中の装備ならEを出す
			if (mEquipmentsData[i].equipChara)
			{
				mEquipmentMark->Render(pos, scale, Vector2::ZERO, size);
			}
		}

		// 選択のフレーム画像を描画
		Vector2 pos(mSelectIndex % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x, mSelectIndex / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y);
		mSelectFrame->Render(pos, scale, Vector2::ZERO, size);
	}

	// Info描画
	{
		if (mEquipmentsData.empty()) return;
		const Vector2 texPos(0.0f, 0.0f);

		// ボード
		const Vector2 infoBoardPos(boardPos.x + INFO_BOARD_OFFSET_X, boardPos.y);
		const Vector2 scale(1.0f, 1.0f);
		mInfoBoard->Render(infoBoardPos, scale, texPos, mInfoBoard->GetSize());

		// アイテムアイコン
		const Vector2 iconPos(infoBoardPos.x + INFO_ICON_OFFSET_X, infoBoardPos.y + INFO_ICON_OFFSET_Y);
		const Vector2 iconScale(INFO_ICON_SCALE, INFO_ICON_SCALE);
		const Vector2 iconSize(ICON_SIZE, ICON_SIZE);

		const EquipmentData::Param param = EquipmentData::GetParam(mEquipmentsData[mSelectIndex].equipmentID);
		param.base->icon->Render(iconPos, iconScale, Vector2::ZERO, iconSize);

		// 説明
		RenderSetInfo(infoBoardPos);
		mFont.Render();
	}


}

void EquipmentSelect::ClearData()
{
	mEquipmentsData.clear();
	mSelectIndex = 0;
}

void EquipmentSelect::RenderSetInfo(const Vector2& infoBoardPos)
{
	const float INFO_OFFSET_X = 32.0f;
	const float NAME_OFFSET_Y = 28;
	const float INFO_OFFSET_Y = 256.0f;
	const float INFO_ADD_Y = 46;

	// 装備品データ取得
	const EquipmentData::Param param = EquipmentData::GetParam(mEquipmentsData[mSelectIndex].equipmentID);

	// 名前
	Vector2 namePos = infoBoardPos + Vector2(mInfoBoard->GetSize().x / 2.0f, NAME_OFFSET_Y);
	Vector2 nameCenter(0.5f, 0.0f);
	mFont.RenderSet(param.base->name.c_str(), namePos, nameCenter, Define::FONT_COLOR);

	// パラメーター
	std::wstring str[PARAM_NUM] =
	{
		L"攻撃力：" + std::to_wstring(param.atk),
		L"防御力：" + std::to_wstring(param.def),
		L"素早さ：" + std::to_wstring(param.spd)
	};

	Vector2 pos;
	Vector2 center(Vector2::ZERO);
	for (int i = 0; i < PARAM_NUM; ++i)
	{
		pos = infoBoardPos + Vector2(INFO_OFFSET_X, INFO_OFFSET_Y + INFO_ADD_Y * i);
		mFont.RenderSet(str[i].c_str(), pos, center, Define::FONT_COLOR);
	}
}