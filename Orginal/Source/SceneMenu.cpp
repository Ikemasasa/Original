#include "SceneMenu.h"

#include "lib/Input.h"

#include "Fade.h"
#include "KeyGuide.h"
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
	mMenuManager.Initialize(mPlayerManager);
	KeyGuide::Instance().ResetMoveX();
}

void SceneMenu::Update()
{
	mMenuManager.Update(mPlayerManager);
}

void SceneMenu::Render()
{
	mMenuManager.Render();
}

void SceneMenu::Release()
{
	mMenuManager.Release();
}
