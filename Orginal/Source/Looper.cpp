#include "Looper.h"

#include "lib/Audio.h"
#include "lib/Font.h"
#include "lib/Framework.h"
#include "lib/Input.h"

#include "Fade.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneField.h"
#include "Singleton.h"
#include "TimeStamp.h"

int Looper::Run(HWND hwnd)
{
	auto& f = FRAMEWORK;
	f.Initialize(hwnd);

	Font::EnableTTF(L"Data/Font/mplus-2p-bold.ttf", L"M+ 2p");

	Input::Initialize();

	Audio::Initialize();

	auto& sm = SceneManager::GetInstance();
	sm.Initialize(std::make_unique<SceneTitle>());

	while (IsLoop() && sm.IsLoop())
	{
		GameManager::elapsedTime = pm.GetElapsedTime();
		Input::Update();
		sm.Update();
		Fade::GetInstance().Update();
		Audio::Update();

		f.Clear();
		sm.Render();
		Fade::GetInstance().Render();

		f.ScreenFlip();
	}
	
	sm.Release();

	Audio::Release();

	SingletonFinalizer::Finalize();
	Font::DisableTTF();

	return 0;
}