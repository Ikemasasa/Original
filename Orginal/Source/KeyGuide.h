#pragma once
#include <memory>
#include <string>
#include <vector>

#include "lib/Font.h"
#include "lib/Sprite.h"

#include "Define.h"

class KeyGuide final
{
	// é¿ë‘çÏê¨ã÷é~
	KeyGuide();
	~KeyGuide() = default;
public:
	enum Key
	{
		A,
		B,
		X,
		Y,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		LB,
		RB,
		DPAD
	};

private:
	static constexpr float ICON_SIZE = 128.0f;
	static constexpr float ICON_SCALE = 0.25;
	static constexpr float ICON_SCALE_SIZE = ICON_SIZE * ICON_SCALE;

	static constexpr float GUIDE_LEFTTOP_X = 12.0f;
	static constexpr float GUIDE_LEFTTOP_Y = 5.0f;
	static constexpr float BOARD_LEFTTOP_Y = Define::SCREEN_HEIGHT - ICON_SCALE_SIZE * 2.0f;

	struct Guide
	{
		std::vector<Key> keys;
		std::wstring description;
	};

private:
	std::unique_ptr<Sprite> mKeyIcons;
	std::unique_ptr<Sprite> mKeyGuideBoard;
	std::vector<Guide> mKeyGuides;
	Font mFont;

public:
	void Add(Key* key, int keyNum, std::wstring description);
	void Render(bool isClear);

	// ÉVÉìÉOÉãÉgÉì
	static KeyGuide& Instance()
	{
		static KeyGuide inst;
		return inst;
	}
};