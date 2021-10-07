#pragma once
#include <memory>
#include <string>
#include <list>

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
		DPAD,
		LB,
		RB,
	};

private:
	static constexpr float ICON_SIZE = 128.0f;
	static constexpr float ICON_SCALE = 0.25;
	static constexpr float ICON_SCALE_SIZE = ICON_SIZE * ICON_SCALE;

	static constexpr float GUIDE_LEFTTOP_X = 12.0f;
	static constexpr float GUIDE_LEFTTOP_Y = 5.0f;
	static constexpr float BOARD_LEFTTOP_Y = Define::SCREEN_HEIGHT - ICON_SCALE_SIZE * 2.0f;

	static constexpr float LERP_FACTOR = 0.1f;

	struct Guide
	{
		std::vector<Key> keys;
		std::wstring description;
	};

private:
	std::unique_ptr<Sprite> mKeyIcons;
	std::unique_ptr<Sprite> mKeyGuideBoard;
	std::list<Guide> mKeyGuides;
	Font mFont;
	float mMoveX = Define::SCREEN_WIDTH;

	std::list<Guide>::iterator GetInsertIterator(Key key);
public:
	void Add(Key key, std::wstring description);
	void Add(Key* key, int keyNum, std::wstring description);
	void Render(bool isClear);
	void ResetMoveX() { mMoveX = Define::SCREEN_WIDTH; };


	// ÉVÉìÉOÉãÉgÉì
	static KeyGuide& Instance()
	{
		static KeyGuide inst;
		return inst;
	}
};