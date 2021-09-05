#pragma once
#include "lib/Font.h"
#include "lib/Vector.h"

#include "Item.h"

class Sprite;

class ItemSelect
{
public:
	static constexpr float ICON_OFFSET = 10.0f; // �{�[�h�̍��ォ��A�C�R���`��ʒu�܂ł̋���(XY����)

private:
	static const int HORIZONTAL_NUM = 5; // �A�C�R������̐�
	static const int VERTICAL_NUM = 6;   // �A�C�R���c��̐�

	static constexpr float ICON_SIZE = 256.0f;
	static constexpr float ICON_SCALE = 0.25f;
		
	static constexpr float INFO_BOARD_OFFSET_X = 512.0f;
	static constexpr float INFO_ICON_OFFSET_X = 114.0f;
	static constexpr float INFO_ICON_OFFSET_Y = 76.0f; 
	static constexpr float INFO_ICON_SCALE = 0.5f;
	static constexpr float INFO_TEXT_OFFSET_X = 25;
	static constexpr float INFO_TEXT_OFFSET_Y = 255;
	static constexpr float INFO_TEXT_ADD_Y = 45;
	static constexpr float INFO_WIDTH_MAX = 300;
	static const int INFO_LINE_MAX = 3;

	int mSelectIndex;
	const Item* mInventory = nullptr;
	std::unique_ptr<Sprite> mItemBoard;
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
	void Update(const Item* inventory);
	void Render(const Vector2& boardOffset);

	int GetIndex() const { return mSelectIndex; }
	Vector2 GetCursorRightUpPos() const; // �{�[�h�̈ʒu���܂܂Ȃ�(0, 0)�
};