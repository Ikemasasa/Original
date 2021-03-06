#include "SelectOptions.h"

#include "lib/Input.h"
#include "lib/Sprite.h"

#include "Define.h"
#include "KeyGuide.h"
#include "Sound.h"

void SelectOptions::Initialize()
{
	// 画像読み込み
	mOptionTitle = std::make_unique<Sprite>(L"Data/Image/Option/option_title.png");
	mOption		 = std::make_unique<Sprite>(L"Data/Image/Option/option.png");
	mOptionEnd	 = std::make_unique<Sprite>(L"Data/Image/Option/option_end.png");
	mOptionSelect = std::make_unique<Sprite>(L"Data/Image/Option/option_select.png");

	// フォント初期化
	mFont.Initialize();
}

void SelectOptions::Update()
{
	// テキストが設定されていなければreturn 
	if (mStrTitle.empty()) return;

	int old = mOptionIndex;
	size_t max = mStrOptions.size();
	if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mOptionIndex = (mOptionIndex + 1) % max;
	if (Input::GetButtonTrigger(0, Input::BUTTON::UP))   mOptionIndex = (mOptionIndex + (max - 1)) % max;

	if (old != mOptionIndex) Sound::Play(Sound::CURSOR_MOVE);

	// キーガイド
	KeyGuide::Key key[] = { KeyGuide::UP, KeyGuide::DOWN };
	KeyGuide::Instance().Add(key, 2, L"カーソル移動");
	KeyGuide::Instance().Add(KeyGuide::A, L"決定");
	KeyGuide::Instance().Add(KeyGuide::B, L"戻る");
}

void SelectOptions::Render(const Vector2& leftTop, bool isStrsClear)
{
	// テキストが設定されていなければreturn 
	if (mStrTitle.empty()) return;

	Vector2 pos = {};
	const Vector2 SCALE(Vector2::ONE);
	const Vector2 TEXPOS(Vector2::ZERO);
	const Vector2 CENTER(Vector2::ZERO);

	//フォントセット
	pos = leftTop + Vector2(mOptionTitle->GetSize().x / 2.0f, TITLE_STR_OFFSET_Y);
	Vector2 titleCenter(0.5f, 0.0f);
	mFont.RenderSet(mStrTitle.c_str(), pos, titleCenter, Define::FONT_COLOR);

	const float TITLE_SIZE_Y = mOptionTitle->GetSize().y;
	size_t loopNum = mStrOptions.size();
	for (size_t i = 0; i < loopNum; ++i)
	{
		pos = leftTop + Vector2(OPTION_STR_OFFSET_X, TITLE_SIZE_Y + mOption->GetSize().y * i);
		mFont.RenderSet(mStrOptions[i].c_str(), pos, CENTER, Define::FONT_COLOR);
	}

	// 描画
	// タイトル
	mOptionTitle->Render(leftTop, SCALE, TEXPOS, mOptionTitle->GetSize());

	// option
	const float OPTION_SIZE_Y = mOption->GetSize().y;
	for (size_t i = 0; i < loopNum; ++i)
	{
		pos = leftTop + Vector2(0.0f, TITLE_SIZE_Y + OPTION_SIZE_Y * i);
		mOption->Render(pos, SCALE, TEXPOS, mOption->GetSize());

		// 選択画像
		if (i == mOptionIndex)
		{
			pos.x += SELECT_OFFSET_X;
			mOptionSelect->Render(pos, SCALE, TEXPOS, mOptionSelect->GetSize());
		}
	}

	// end
	pos = leftTop + Vector2(0.0f, TITLE_SIZE_Y + OPTION_SIZE_Y * loopNum);
	mOptionEnd->Render(pos, SCALE, TEXPOS, mOptionEnd->GetSize());
	
	// フォント
	mFont.Render();

	if (isStrsClear)
	{
		mStrTitle.clear();
		mStrOptions.clear();
	}
}

void SelectOptions::AddTitle(const wchar_t* str)
{
	mStrTitle = str;
}

void SelectOptions::AddOption(const wchar_t* str)
{
	mStrOptions.emplace_back(str);
}