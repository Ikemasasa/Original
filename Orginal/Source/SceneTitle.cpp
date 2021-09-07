#include "SceneTitle.h"

#include "lib/Audio.h"
#include "lib/Input.h"

#include "Define.h"
#include "Fade.h"
#include "SceneField.h"
#include "SceneManager.h"

SceneTitle::SceneTitle()
{

}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Initialize()
{
	mBG = std::make_unique<Sprite>(L"Data/Image/title_bg.png");
	mBoard = std::make_unique<Sprite>(L"Data/Image/Menu/str_board.png");
	mBoardSelect = std::make_unique<Sprite>(L"Data/Image/Menu/str_select.png");

	mFont.Initialize();
	mFontLogo.Initialize(FONT_LOGO_SIZE, FONT_LOGO_WEIGHT);

	mIsPressAButton = false;
	mSelectIndex = 0;

	AUDIO.MusicPlay((int)Music::TITLE);
}

void SceneTitle::Update()
{
	// ロゴ
	const wchar_t* logoStr = L"R P G";
	Vector2 pos(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.20f);
	Vector2 center(0.5f, 0.0f);
	mFontLogo.RenderSet(logoStr, pos, center, Define::FONT_COLOR);

	// 選択する文字列
	if (!mIsPressAButton)
	{
		const wchar_t* pressAButton = L"PRESS A BUTTON";
		float y = Define::SCREEN_HEIGHT * 0.25f;
		Vector2 pos(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.7f);
		Vector2 center(0.5f, 0.5f);
		Vector4 color(Define::FONT_COLOR);
		mSinAngle += Define::PI / 40.0f; // 数値は適当
		color.w = sinf(mSinAngle) + 1.0f * 0.5f;
		mFont.RenderSet(pressAButton, pos, center, color);

		if (Input::GetButtonTrigger(0, Input::A))
		{
			mIsPressAButton = true;
			AUDIO.SoundPlay((int)Sound::SELECT_LONG);
		}
	}
	else
	{
		// GAME_START(0) ~ EXIT(1)に抑える
		int old = mSelectIndex;
		if (Input::GetButtonTrigger(0, Input::UP))   mSelectIndex = (mSelectIndex + MAX - 1) % MAX;
		if (Input::GetButtonTrigger(0, Input::DOWN)) mSelectIndex = (mSelectIndex + 1) % MAX;
		if (old != mSelectIndex) AUDIO.SoundPlay((int)Sound::CURSOR_MOVE);

		if (Input::GetButtonTrigger(0, Input::A))
		{
			Fade::GetInstance().Set(Fade::SPEED_SLOW);
			AUDIO.SoundPlay((int)Sound::SELECT);
		}
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

	if (mIsPressAButton)
	{
		const wchar_t* SelectStr[MAX] =
		{
			L"GAME START",
			L"EXIT",
		};
		// ボードの描画
		float d = Define::SCREEN_HEIGHT - Define::SCREEN_HEIGHT / 2.0f;
		const float OFFSET_Y = (d - mBoard->GetSize().y * MAX) / (MAX + 1);
		for (int i = 0; i < MAX; ++i)
		{
			Vector2 center(mBoard->GetSize().x / 2.0f, 0.0f);
			Vector2 pos(Define::SCREEN_WIDTH / 2.0f, Define::SCREEN_HEIGHT / 2.0f + OFFSET_Y * (i + 1));
			mBoard->Render(pos, Vector2::ONE, Vector2::ZERO, mBoard->GetSize(), center);
			if (mSelectIndex == i)
			{
				mBoardSelect->Render(pos, Vector2::ONE, Vector2::ZERO, mBoardSelect->GetSize(), center);
			}

			// フォントセット
			pos.y += STR_OFFSET;
			center.x = 0.5f;
			mFont.RenderSet(SelectStr[i], pos, center, Define::FONT_COLOR);
		}
	}

	mFont.Render();
	mFontLogo.Render();
}

void SceneTitle::Release()
{
	mFont.Release();

	AUDIO.MusicStop((int)Music::TITLE);
}
