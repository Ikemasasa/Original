#pragma once
#include "lib/Vector.h"

#include "Item.h"

class Texture;

class ItemSelect
{
	static const int HORIZONTAL_NUM = 5; // �A�C�R������̐�
	static const int VERTICAL_NUM = 6;   // �A�C�R���c��̐�

	static constexpr float ICON_OFFSET = 19.0f; // �{�[�h�̍��ォ��A�C�R���`��ʒu�܂ł̋���(XY����)
	static constexpr float ICON_SIZE = 256.0f;
	static constexpr float ICON_SCALE = 0.25f;

	int mSelectIndex;
	int mOldSelectIndex;
	const Item* mInventory;
	std::unique_ptr<Texture> mBoard;
	std::unique_ptr<Texture> mSelectFrame;

public:
	ItemSelect() = default;
	~ItemSelect() = default;

	void Initialize();
	int Update(const Item* inventory);
	void Render(const Vector2& boardOffset);
};