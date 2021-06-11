#include "Looper.h"

#include "lib/Audio.h"
#include "lib/Font.h"
#include "lib/Framework.h"
#include "lib/Input.h"

#include "GameManager.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneField.h"
#include "Fade.h"

int Looper::Run(HWND hwnd)
{
	auto& f = FRAMEWORK;
	f.Initialize(hwnd);

	Font::EnableTTF(L"Data/Font/mplus-2p-bold.ttf", L"M+ 2p");

	Input::Initialize();

	AUDIO.Initialize();

	auto& sm = SceneManager::GetInstance();
	sm.Initialize(std::make_unique<SceneField>());

	while (IsLoop() && sm.IsLoop())
	{
		GameManager::elpsedTime = pm.GetElapsedTime();

		Input::Update();
		sm.Update();
		Fade::GetInstance().Update();

		f.Clear();
		sm.Render();
		Fade::GetInstance().Render();

		f.ScreenFlip();
	}
	
	sm.Release();

	Font::DisableTTF();

	return 0;
}