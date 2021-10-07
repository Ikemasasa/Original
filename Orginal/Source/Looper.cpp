#include "Looper.h"

#include "lib/Audio.h"
#include "lib/Font.h"
#include "lib/Framework.h"
#include "lib/Input.h"

#include "Fade.h"
#include "GameManager.h"
#include "KeyGuide.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "Singleton.h"

int Looper::Run(HWND hwnd)
{
	auto& f = FRAMEWORK;
	f.Initialize(hwnd);

	Font::EnableTTF(L"Data/Font/mplus-2p-bold.ttf", L"M+ 2p");

	Input::Initialize();

	Audio::Initialize();

	auto& sm = SceneManager::GetInstance();
	sm.Initialize(std::make_unique<SceneTitle>());

	while (pm.IsLoop() && sm.IsLoop())
	{
		if (!pm.IsProcess()) continue;

		GameManager::elapsedTime = pm.GetElapsedTime();
		Input::Update();
		sm.Update();
		Fade::GetInstance().Update();
		Audio::Update();

		f.Clear();
		sm.Render();
		Fade::GetInstance().Render();
		KeyGuide::Instance().Render(true);

		f.ScreenFlip();
	}
	
	sm.Release();

	Audio::Release();

	SingletonFinalizer::Finalize();
	Font::DisableTTF();

	return 0;
}