#pragma once
#include "lib/Font.h"
#include "lib/Vector.h"

#include "Item.h"

class Sprite;

class ItemSelect
{
	static const int HORIZONTAL_NUM = 5; // アイコン横列の数
	static const int VERTICAL_NUM = 6;   // アイコン縦列の数

	static constexpr float ICON_OFFSET = 19.0f; // ボードの左上からアイコン描画位置までの距離(XY両方)
	static constexpr float ICON_SIZE = 256.0f;
	static constexpr float ICON_SCALE = 0.25f;
		
	static constexpr float INFO_BOARD_OFFSET_X = 512.0f;
	static constexpr float INFO_ICON_OFFSET_X = 114.0f;
	static constexpr float INFO_ICON_OFFSET_Y = 76.0f; // このオフセットとかはまじでGIMPとかで見てやってる、サイズ考えてアセット作るの大変だね
	static constexpr float INFO_ICON_SCALE = 0.5f;

	int mSelectIndex;
	int mOldSelectIndex ;
	const Item* mInventory = nullptr;
	std::unique_ptr<Sprite> mBoard;
	std::unique_ptr<Sprite> mSelectFrame;
	std::unique_ptr<Sprite> mInfoBoard;
	std::vector<std::wstring> mInfo;
	Font mFont;

private:
	void UpdateInfo();
	void RenderSetInfo(const Vector2& infoBoardPos);

public:
	ItemSelect() = default;
	~ItemSelect() = default;

	void Initialize();
	int Update(const Item* inventory);
	void Render(const Vector2& boardOffset);
};