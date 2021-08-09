#pragma once
#include <memory>
#include <vector>

#include "lib/Font.h"

#include "EquipmentInventory.h"
#include "FontValue.h"

class Sprite;

class EquipmentSelect
{
	static const int HORIZONTAL_NUM = 5; // �A�C�R������̐�
	static const int VERTICAL_NUM = 6;   // �A�C�R���c��̐�
	static const int PARAM_NUM = 3;

	static constexpr float ICON_OFFSET = 19.0f; // �{�[�h�̍��ォ��A�C�R���`��ʒu�܂ł̋���(XY����)
	static constexpr float ICON_SIZE = 256.0f;
	static constexpr float ICON_SCALE = 0.25f;

	static constexpr float INFO_BOARD_OFFSET_X = 400.0f;
	static constexpr float INFO_ICON_OFFSET_X = 114.0f;
	static constexpr float INFO_ICON_OFFSET_Y = 76.0f; // ���̃I�t�Z�b�g�Ƃ��͂܂���GIMP�Ƃ��Ō��Ă���Ă�A�T�C�Y�l���ăA�Z�b�g���̑�ς���
	static constexpr float INFO_ICON_SCALE = 0.5f;

	std::unique_ptr<Sprite> mBoard;
	std::unique_ptr<Sprite> mSelectFrame;
	std::unique_ptr<Sprite> mInfoBoard;
	int mSelectIndex = 0;
	int mOldSelectIndex = 0;
	std::vector<EquipmentInventory::Data> mEquipmentsData;

	Font mFont;

	std::vector<std::wstring> mInfo;

private:
	void RenderSetInfo(const Vector2& infoBoardPos);

public:
	void Initialize();
	int Update(const std::vector<EquipmentInventory::Data>& equipmentsData);
	void Render(const Vector2& boardPos);
};