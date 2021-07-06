#pragma once
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

	int mSelectIndex = 0;
	int mOldSelectIndex = 0;
	const Item* mInventory = nullptr;
	std::unique_ptr<Sprite> mBoard;
	std::unique_ptr<Sprite> mSelectFrame;

public:
	ItemSelect() = default;
	~ItemSelect() = default;

	void Initialize();
	int Update(const Item* inventory);
	void Render(const Vector2& boardOffset) const;
};