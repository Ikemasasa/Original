#pragma once
#include "lib/Font.h"
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
		
	static constexpr float INFO_BOARD_OFFSET_X = 512.0f;
	static constexpr float INFO_ICON_OFFSET_X = 114.0f;
	static constexpr float INFO_ICON_OFFSET_Y = 76.0f; // ���̃I�t�Z�b�g�Ƃ��͂܂���GIMP�Ƃ��Ō��Ă���Ă�A�T�C�Y�l���ăA�Z�b�g���̑�ς���
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