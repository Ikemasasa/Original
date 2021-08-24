#pragma once
#include <memory>
#include <string>

#include "lib/Font.h"

#include "FontValue.h"

class PlayerManager;
class Sprite;
struct Status;

class CharacterHealth
{
	static const int STATUS_NUM = 2;
	static constexpr float SELECT_OFFSET = 5;

	// •¶Žš‚ÌˆÊ’u
	static constexpr float FIRST_OFFSET_X = 16.0f;
	static constexpr float CUR_OFFSET_X = 100.0f;
	static constexpr float DELIM_OFFSET_X = 4.0f;
	static constexpr float MAX_OFFSET_X = 64.0f;

	static constexpr float FIRST_OFFSET_Y = 12.0f;
	static constexpr float ADD_OFFSET_Y = 32.0f;

private:
	std::unique_ptr<Sprite> mHealthBoard;
	std::unique_ptr<Sprite> mSelect;

	Font mFont;
	FontValue mFontValue;
	Vector2 mBoardLeftTop;
	int mSelectIndex;
	int mBoardNum;

public:
	CharacterHealth() = default;
	~CharacterHealth() = default;

	void Initialize(const Vector2& leftTop);
	void Update(const std::vector<Status>& statusArray);
	void Render(bool isSelectRender = false);
	void Release();

	int GetSelectIndex() const { return mSelectIndex; }
};