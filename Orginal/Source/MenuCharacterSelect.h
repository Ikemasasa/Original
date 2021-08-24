#pragma once
#include <memory>

#include "lib/Font.h"

class PlayerManager;
class Sprite;

class MenuCharacterSelect
{
	static constexpr float FONT_OFFSET_X = 5;
	static constexpr float FONT_OFFSET_Y = 32; // フォントサイズ

	std::unique_ptr<Sprite> mBoard;
	int mCharacterNum = 0;
	int mSelectIndex = 0;
	int mOldSelectIndex = 0;

	Font mFont;

public:
	void Initialize(const PlayerManager* plm);
	void Update();
	void Render(Vector2 leftBottom);
	void Release();

	bool IsChangedIndex()const { return mSelectIndex != mOldSelectIndex; }

	int GetIndex() const { return mSelectIndex; }
};