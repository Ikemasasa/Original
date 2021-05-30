#pragma once
#include "MenuBase.h"

#include <memory>
#include <vector>

#include "lib/Font.h"

class MenuCharacterSelect;
class PlayerManager;
class Texture;
class Item;

class MenuItem : public MenuBase
{
	enum State
	{
		INVENTORY_SELECT,
		ITEM_SELECT,
		TARGET_SELECT
	};

	struct IconData
	{
		int id;
		std::shared_ptr<Texture> tex;
	};

	static const int HORIZONTAL_NUM = 5; // アイコン横列の数
	static const int VERTICAL_NUM = 6;   // アイコン縦列の数

	static constexpr float ICON_OFFSET = 19.0f; // ボードの左上からアイコン描画位置までの距離(XY両方)
	static constexpr float ICON_SIZE = 256.0f;
	static constexpr float ICON_SCALE = 0.25f;

	static constexpr float BOARD_OFFSET_X = 100.0f;
	static constexpr float BOARD_OFFSET_Y = 150.0f;

private:
	Font mFont;
	PlayerManager* mPlayerManager;
	Item* mSelectInventory;
	std::vector<IconData> mIcons;
	std::unique_ptr<Texture> mSelect;
	std::unique_ptr<Texture> mBoard;
	std::unique_ptr<MenuCharacterSelect> mCharacterSelect;
	State mState;

	void InventorySelect(); // 誰のインベントリを参照するか選択
	void ItemSelect();		// どのアイテムを使うか選択
	void TargetSelect();	// アイテムを誰に使うか選択

	void RenderItem();

public:
	MenuItem(PlayerManager* plm);
	~MenuItem();

	Select Update(PlayerManager* plm);
	void Render();
	void Release();
};