#include "SceneTitle.h"
#include "SceneField.h"
#include "SceneManager.h"
#include "Fade.h"
#include "Define.h"
#include "lib/Input.h"
#include "lib/DXTKAudio.h"

void SceneTitle::Initialize()
{
	mSprite = new Texture(L"Data/Image/titleScreen.png");
}

void SceneTitle::Update()
{
	if (Input::GetButton(0, Input::A))
	{
		Fade::GetInstance().Set(0.05f);
	}

	if (Fade::GetInstance().IsFadeOutEnd())
	{
		SceneManager::GetInstance().SetNextScene(std::make_unique<SceneField>());
	}
}

void SceneTitle::Render()
{
	mSprite->Render(Vector2::Zero(), Vector2::One(), Vector2::Zero(), Vector2(Define::SCREEN_WIDTH, Define::SCREEN_HEIGHT), Vector2::Zero());
}

void SceneTitle::Release()
{
	delete mSprite;
	mSprite = nullptr;
}
