#include "SceneMenu.h"

#include "lib/Input.h"

#include "Fade.h"
#include "PlayerManager.h"
#include "SceneManager.h"

SceneMenu::SceneMenu(PlayerManager* plm)
{
	mPlayerManager = plm;
}

SceneMenu::~SceneMenu()
{
}

void SceneMenu::Initialize()
{
	mMenuManager.Initialize();
}

void SceneMenu::Update()
{
	mMenuManager.Update(mPlayerManager);

	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		if (Fade::GetInstance().SetSceneImage(Fade::SPEED_FAST))
		{
			SceneManager::GetInstance().PopCurrentScene();
		}
	}
}

void SceneMenu::Render()
{
	mMenuManager.Render();
}

void SceneMenu::Release()
{
	mMenuManager.Release();
}
