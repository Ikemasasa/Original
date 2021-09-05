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
	static constexpr float ICON_SIZE = 256.0f;
	static constexpr float ICON_SCALE = 0.125f;
	static constexpr float ICON_SCALESIZE = ICON_SIZE * ICON_SCALE;

	static constexpr float ARROW_SIZE_X = 128.0f;
	static constexpr float ARROW_SIZE_Y = 256.0f;
	static constexpr float ARROW_SCALE = 0.125f;
	static constexpr float ARROW_SCALESIZE_X = ARROW_SIZE_X * ICON_SCALE;
	static constexpr float ARROW_SCALESIZE_Y = ARROW_SIZE_Y * ICON_SCALE;
	static constexpr float ARROW_TEXPOS_X = 768.0f;

	// 文字の位置
	static constexpr float NAME_OFFSET_X = 35;
	static constexpr float NAME_OFFSET_Y = 5;
	static constexpr float STATUS_OFFSET_X = 16;
	static constexpr float STATUS_OFFSET_Y = 38;
	static constexpr float STATUS_ADD_Y = 33;

	static constexpr float CUR_OFFSET_X = 80.0f;
	static constexpr float DELIM_OFFSET_X = 4.0f;

	struct BuffData
	{
		int buffIndex = 0; // どのバフを指すか(具体的に、0:攻撃 1:防御)
		Vector2 pos;//アイコン描画位置
	};

private:
	std::unique_ptr<Sprite> mHealthBoard;
	std::unique_ptr<Sprite> mSelect;
	std::unique_ptr<Sprite> mBuffIcon;

	Font mFont;
	FontValue mFontValue;
	Vector2 mBoardLeftTop;
	std::vector<std::vector<BuffData>> mBuffData;
	std::vector<std::vector<BuffData>> mDebuffData;
	int mSelectIndex;
	int mBoardNum;

public:
	CharacterHealth() = default;
	~CharacterHealth() = default;

	void Initialize(const Vector2& leftTop);
	void Update(const std::vector<Status>& statusArray);
	void Render(bool isSelectRender = false, bool isFontClear = true);
	void Release();

	// ゲッタ
	int GetSelectIndex() const { return mSelectIndex; }
	Vector2 GetBoardSize() const;

	// セッタ
	void SetLeftTopPos(const Vector2& leftTop) { mBoardLeftTop = leftTop; }
};