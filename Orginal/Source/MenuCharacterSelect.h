#pragma once
#include <memory>

#include "lib/Font.h"

class PlayerManager;
class Sprite;

class MenuCharacterSelect
{
	static constexpr float FONT_OFFSET_X = 2;
	static constexpr float FONT_OFFSET_Y = 32; // フォントサイズ / 2

	std::unique_ptr<Sprite> mBoard;
	int mSelectIndex = 0;
	int mOldSelectIndex = 0;
	std::vector<wchar_t> mPlayerNameFirst;
	Font mFont;

public:
	void Initialize(const PlayerManager* plm);
	void Update();
	void Render(Vector2 leftBottom);
	void Release();

	bool IsChangedIndex()const { return mSelectIndex != mOldSelectIndex; }

	int GetIndex() const { return mSelectIndex; }
};