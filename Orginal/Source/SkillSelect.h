#pragma once
#include <memory>
#include <vector>

#include "lib/Font.h"

#include "FontValue.h"
#include "SkillData.h"

class Sprite;

class SkillSelect
{
	static constexpr float SKILLNAME_POS_X = 18.0f;
	static constexpr float SKILLNAME_POS_Y = 18.0f;
	static constexpr float USE_MP_OFFSET_X = 280.0f;
	static const int VERTICAL_SKILL_MAX = 6;
	static const int FONT_SIZE = 32;

	static constexpr float SKILLINFO_POS_X = 14.0f;
	static constexpr float SKILLINFO_POS_Y = 229.0f;

private:
	std::unique_ptr<Sprite> mSkillBoard;
	std::unique_ptr<Sprite> mSkillSelect;
	std::vector<SkillData::SkillParam> mParams;
	int mSkillIndex = 0;
	Vector2 mBoardPos;
	Font mFont;
	FontValue mFontValue;

public:
	void Initialize(const int charaID, const Vector2& leftTop);
	void Update();
	void Render();

	const SkillData::SkillParam* GetSelectSkill() const { return &mParams[mSkillIndex]; }
	int GetIndex() const { return mSkillIndex; }

	bool ExistSkill() const { return !mParams.empty(); }
};