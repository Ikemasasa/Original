#include "SceneTitle.h"

#include "lib/Audio.h"
#include "lib/Input.h"

#include "Define.h"
#include "Fade.h"
#include "SceneField.h"
#include "SceneManager.h"

SceneTitle::SceneTitle()
{
	const wchar_t* addLogoStr = L"R P G";

	// + 1はPRESS~のため
	const int SELECT_STR_NUM = MAX + 1;
	const wchar_t* addSelectstr[SELECT_STR_NUM] =
	{
		L"GAME START",
		L"EXIT",
		L"PRESS A BUTTON",
	};

	mFontLogo.Initialize(LOGO_FONT_SIZE, LOGO_FONT_WEIGHT);
	mFontLogo.Add(addLogoStr);

	mFontSelectStr.Initialize(SELECT_FONT_SIZE, SELECT_FONT_WEIGHT);
	for (int i = 0; i < SELECT_STR_NUM; ++i) mFontSelectStr.Add(addSelectstr[i]);

	mBG = std::make_unique<Sprite>(L"Data/Image/title_bg.png");
	mArrow = std::make_unique<Sprite>(L"Data/Image/arrow.png");
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Initialize()
{
	AUDIO.MusicPlay((int)Music::TITLE);

	mIsPressAButton = false;
	mSelectIndex = 0;
}

void SceneTitle::Update()
{
	// ロゴ
	const int logoIndex = 0;
	float strWidth = mFontLogo.GetWidth(logoIndex);
	Vector2 pos(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT - Define::SCREEN_HEIGHT * 0.85f);
	Vector2 center(strWidth * 0.5f, 0.5f);
	mFontLogo.RenderSet(logoIndex, pos, center);

	if (!mIsPressAButton)
	{
		const int PRESS_A_KEY_INDEX = 2;
		float strWidth = mFontSelectStr.GetWidth(PRESS_A_KEY_INDEX);
		Vector2 pos(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.60f);
		Vector2 center(strWidth * 0.5f, 0.5f);
		mFontSelectStr.RenderSet(PRESS_A_KEY_INDEX, pos, center);

		if (Input::GetButtonTrigger(0, Input::A)) mIsPressAButton = true;
	}
	else
	{
		// GAME_START(1) ~ EXIT(2)に抑える
		if (Input::GetButtonTrigger(0, Input::UP))   mSelectIndex = (mSelectIndex + MAX - 1) % MAX;
		if (Input::GetButtonTrigger(0, Input::DOWN)) mSelectIndex = (mSelectIndex + 1) % MAX;

		// 選択する文字描画
		for (int i = GAME_START; i < MAX; ++i)
		{
			float strWidth = mFontSelectStr.GetWidth(i);
			Vector2 pos(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f + SELECT_FONT_SIZE * i);
			Vector2 center(strWidth * 0.5f, 0.0f);
			mFontSelectStr.RenderSet(i, pos, center);
		}

		if (Input::GetButtonTrigger(0, Input::A)) Fade::GetInstance().Set(Fade::SPEED_SLOW);
		if (Fade::GetInstance().IsFadeOutEnd())
		{
			switch (mSelectIndex)
			{
			case GAME_START: SceneManager::GetInstance().SetNextScene(std::make_unique<SceneField>()); break;
			case EXIT: SceneManager::GetInstance().PopCurrentScene(); break;
			}
		}
	}


}

void SceneTitle::Render()
{
	mBG->Render(Vector2::ZERO, Vector2::ONE, Vector2::ZERO, mBG->GetSize());

	mFontLogo.Render();
	mFontSelectStr.Render();

	if (mIsPressAButton)
	{
		Vector2 pos(Define::SCREEN_WIDTH * 0.3f, Define::SCREEN_HEIGHT * 0.5f + SELECT_FONT_SIZE * mSelectIndex);
		mArrow->Render(pos, Vector2::ONE, Vector2::ZERO, mArrow->GetSize());
	}
}

void SceneTitle::Release()
{
	mFontLogo.Release();
	mFontSelectStr.Release();

	AUDIO.MusicStop((int)Music::TITLE);
}
