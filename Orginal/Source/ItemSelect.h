#pragma once
#include "lib/Vector.h"

#include "Item.h"

class Sprite;

class ItemSelect
{
	static const int HORIZONTAL_NUM = 5; // �A�C�R������̐�
	static const int VERTICAL_NUM = 6;   // �A�C�R���c��̐�

	static constexpr float ICON_OFFSET = 19.0f; // �{�[�h�̍��ォ��A�C�R���`��ʒu�܂ł̋���(XY����)
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