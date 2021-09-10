#include "SceneLoad.h"

#include <thread>

#include "lib/Audio.h"
#include "lib/Sprite.h"

#include "Define.h"
#include "Fade.h"
#include "SceneManager.h"

SceneLoad::SceneLoad(std::unique_ptr<SceneBase> loadScene)
{
	mLoadScene.reset(loadScene.release());
	mStr = std::make_unique<Sprite>(L"Data/Image/now_loding.png");
	mBG = std::make_unique<Sprite>(L"Data/Image/title_bg.png");
}

void SceneLoad::Initialize()
{
	auto Load = [&]() { mLoadScene->Initialize(); };
	// スレッド開始
	std::thread th(Load);
	th.detach();
}

void SceneLoad::Update()
{
	if (mLoadScene->IsLoadFinished())
	{
		// フェード開始
		Fade::GetInstance().Set(Fade::SPEED_FAST);

		// フェードアウトが終わったら
		if (Fade::GetInstance().IsFadeOutEnd())
		{
			SceneManager::GetInstance().PopCurrentScene();
			SceneManager::GetInstance().SetStackScene(std::move(mLoadScene));
		}
	}

}

void SceneLoad::Render()
{
	mBG->Render(Vector2::ZERO, Vector2::ONE, Vector2::ZERO, mBG->GetSize());

	float y = TEXT_Y + sinf(mSinAngle) * SIN_MUL;
	mSinAngle += Define::PI / 50.0f;
	mStr->Render(Vector2(TEXT_X, y), Vector2::ONE, Vector2::ZERO, mStr->GetSize());
}

void SceneLoad::Release()
{

}
