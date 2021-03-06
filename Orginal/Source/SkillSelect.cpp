#include "SkillSelect.h"

#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "Define.h"
#include "KeyGuide.h"
#include "Sound.h"

void SkillSelect::Initialize(const int charaID, const Vector2& leftTop)
{
	// 画像読み込み
	mSkillBoard = std::make_unique<Sprite>(L"Data/Image/Menu/skill_board.png");
	mSkillSelect = std::make_unique<Sprite>(L"Data/Image/Menu/skill_select.png");

	// 使用可能なスキルを保存
	mSkills = SkillData::GetCharaSkill(charaID);
	mSkillIndex = 0;
	mBoardPos = leftTop;

	// フォントクラス初期化
	mFont.Initialize();
}

void SkillSelect::Update()
{
	int num = mSkills.size();
	if (num > 0)
	{
		int oldIndex = mSkillIndex;
		if (Input::GetButtonTrigger(0, Input::BUTTON::UP)) mSkillIndex = (mSkillIndex + (num - 1)) % num;
		if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mSkillIndex = (mSkillIndex + 1) % num;
		if (oldIndex != mSkillIndex) Sound::Play(Sound::CURSOR_MOVE);

		// スキル名、消費MP
		int loopNum = Math::Min(VERTICAL_SKILL_MAX, num);
		const wchar_t* str = L"消費MP:";
		for (int i = 0; i < loopNum; ++i)
		{
			Vector2 pos = {};
			pos.x = mBoardPos.x + SKILLNAME_POS_X;
			pos.y = mBoardPos.y + SKILLNAME_POS_Y + i * FONT_SIZE;
			Vector2 center = Vector2::ZERO;
			mFont.RenderSet(mSkills[i]->name.c_str(), pos, center, Define::FONT_COLOR);

			pos.x += USE_MP_OFFSET_X;
			mFont.RenderSet(str, pos, center, Define::FONT_COLOR);
			pos.x += mFont.GetWidth(str);
			mFont.RenderSetValue(mSkills[i]->useMP, pos, center, Define::FONT_COLOR);
		}

		// スキル情報(白字にしてみる
		int lineIndex = 0;
		std::wstring line;
		for (size_t i = 0; i < mSkills[mSkillIndex]->info.size(); ++i)
		{
			float width = mFont.GetWidth((line + mSkills[mSkillIndex]->info[i]).c_str());

			if (width >= INFO_LINE_PX)
			{
				Vector2 pos(mBoardPos.x + SKILLINFO_POS_X, mBoardPos.y + SKILLINFO_POS_Y + (lineIndex * FONT_SIZE + 2));
				Vector2 center(Vector2::ZERO);
				mFont.RenderSet(line.c_str(), pos, center);

				++lineIndex;
				line.clear();
			}

			line += mSkills[mSkillIndex]->info[i];
		}
		Vector2 pos(mBoardPos.x + SKILLINFO_POS_X, mBoardPos.y + SKILLINFO_POS_Y + (lineIndex * FONT_SIZE + 2));
		Vector2 center(Vector2::ZERO);
		mFont.RenderSet(line.c_str(), pos, center);

		//キーガイド
		KeyGuide::Instance().Add(KeyGuide::A, L"決定");
		KeyGuide::Instance().Add(KeyGuide::B, L"戻る");
		if (num >= 2)
		{
			KeyGuide::Key key[] = { KeyGuide::UP, KeyGuide::DOWN };
			KeyGuide::Instance().Add(key, 2, L"カーソル移動");
		}

	}
	else
	{
		// スキルがない
		Vector2 pos(mBoardPos.x + SKILLINFO_POS_X, mBoardPos.y + SKILLINFO_POS_Y);
		Vector2 center(Vector2::ZERO);
		mFont.RenderSet(L"スキルはありません", pos, center);

		KeyGuide::Instance().Add(KeyGuide::B, L"戻る");
	}
}

void SkillSelect::Render()
{
	// プレートを描画
	mSkillBoard->Render(mBoardPos, Vector2::ONE, Vector2::ZERO, mSkillBoard->GetSize());

	// 選択画像を描画
	if (mSkills.size() > 0)
	{
		Vector2 pos(mBoardPos.x + SKILLNAME_POS_X, mBoardPos.y + SKILLNAME_POS_Y + mSkillIndex * FONT_SIZE);
		mSkillSelect->Render(pos, Vector2::ONE, Vector2::ZERO, mSkillSelect->GetSize());
	}

	// フォントを描画
	mFont.Render();
}
