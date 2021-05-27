#pragma once
#include <memory>
#include <vector>

#include "lib/Font.h"

class PlayerManager;
class Texture;

class MenuItem
{
	static const int HORIZONTAL_NUM = 5; // アイコン横列の数
	static const int VERTICAL_NUM = 6;   // アイコン縦列の数

	static constexpr float ICON_OFFSET = 19.0f; // ボードの左上からアイコン描画位置までの距離(XY両方)
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