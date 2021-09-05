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
	AUDIO.MusicPlay((int)Music::TITLE);

	mIsPressAButton = false;
	mSelectIndex = 0;

	const wchar_t* addLogoStr = L"R P G";

	mFont.Initialize(FONT_SIZE, FONT_WEIGHT);
	mBG = std::make_unique<Sprite>(L"Data/Image/title_bg.png");
	mArrow = std::make_unique<Sprite>(L"Data/Image/arrow.png");
}

void SceneTitle::Update()
{

	// ÉçÉS
	const wchar_t* logoStr = L"R P G";
	float strWidth = mFont.GetWidth(logoStr);
	Vector2 pos(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT - Define::SCREEN_HEIGHT * 0.85f);
	Vector2 center(0.5f, 0.0f);
	mFont.RenderSet(logoStr, pos, center);


	// ëIëÇ∑ÇÈï∂éöóÒ
	if (!mIsPressAButton)
	{
		const wchar_t* pressAButton = L"PRESS A BUTTON";
		Vector2 pos(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.60f);
		Vector2 center(0.5f, 0.5f);
		mFont.RenderSet(pressAButton, pos, center);

		if (Input::GetButtonTrigger(0, Input::A)) mIsPressAButton = true;
	}
	else
	{
		const wchar_t* SelectStr[MAX] =
		{
			L"GAME START",
			L"EXIT",
		};

		// GAME_START(0) ~ EXIT(1)Ç…ó}Ç¶ÇÈ
		if (Input::GetButtonTrigger(0, Input::UP))   mSelectIndex = (mSelectIndex + MAX - 1) % MAX;
		if (Input::GetButtonTrigger(0, Input::DOWN)) mSelectIndex = (mSelectIndex + 1) % MAX;

		// ëIëÇ∑ÇÈï∂éöï`âÊ
		for (int i = 0; i < MAX; ++i)
		{
			float strWidth = mFont.GetWidth(SelectStr[i]);
			Vector2 pos(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f + FONT_SIZE * i);
			Vector2 center(0.5f, 0.0f);
			mFont.RenderSet(SelectStr[i], pos, center);
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
	mFont.Render();

	if (mIsPressAButton)
	{
		Vector2 pos(Define::SCREEN_WIDTH * 0.3f, Define::SCREEN_HEIGHT * 0.5f + FONT_SIZE * mSelectIndex);
		mArrow->Render(pos, Vector2::ONE, Vector2::ZERO, mArrow->GetSize());
	}
}

void SceneTitle::Release()
{
	mFont.Release();

	AUDIO.MusicStop((int)Music::TITLE);
}
