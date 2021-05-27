#pragma once
#include <memory>
#include <vector>

#include "lib/Font.h"

class PlayerManager;
class Texture;

class MenuItem
{
	static const int HORIZONTAL_NUM = 5; // �A�C�R������̐�
	static const int VERTICAL_NUM = 6;   // �A�C�R���c��̐�

	static constexpr float ICON_OFFSET = 19.0f; // �{�[�h�̍��ォ��A�C�R���`��ʒu�܂ł̋���(XY����)
	static constexpr float ICON_SIZE = 256.0f;
	static constexpr float ICON_SCALE = 0.25f;

	static constexpr float BOARD_OFFSET_X = 100.0f;
	static constexpr float BOARD_OFFSET_Y = 150.0f;

	struct IconData
	{
		int id;
		std::shared_ptr<Texture> tex;
	};

	Font mFont;
	PlayerManager* mPlayerManager;

	std::vector<IconData> mIcons;
	std::unique_ptr<Texture> mSelect;
	std::unique_ptr<Texture> mBoard;
	int mSelectIndex = 0;

	void RenderIcons();
public:
	MenuItem(PlayerManager* plm);
	~MenuItem();

	void Update();
	void Render();
	void Release();
};