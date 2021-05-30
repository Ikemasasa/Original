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

	static const int HORIZONTAL_NUM = 5; // �A�C�R������̐�
	static const int VERTICAL_NUM = 6;   // �A�C�R���c��̐�

	static constexpr float ICON_OFFSET = 19.0f; // �{�[�h�̍��ォ��A�C�R���`��ʒu�܂ł̋���(XY����)
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

	void InventorySelect(); // �N�̃C���x���g�����Q�Ƃ��邩�I��
	void ItemSelect();		// �ǂ̃A�C�e�����g�����I��
	void TargetSelect();	// �A�C�e����N�Ɏg�����I��

	void RenderItem();

public:
	MenuItem(PlayerManager* plm);
	~MenuItem();

	Select Update(PlayerManager* plm);
	void Render();
	void Release();
};