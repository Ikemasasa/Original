#include "MenuManager.h"

#include "lib/Input.h"
#include "lib/Texture.h"

#include "Fade.h"
#include "MenuItem.h"
#include "MenuStatus.h"
#include "SceneManager.h"

MenuManager::MenuManager()
{
	mBG = std::make_unique<Texture>(L"Data/Image/Menu/Menu_BG.png");

	mMenuStack.emplace(new MenuSelect);

	mState = MenuBase::NONE;
}

void MenuManager::Initialize()
{
}

void MenuManager::Update(PlayerManager* plm)
{
	// 次のメニューがあれば更新する
	if (mNextMenu) mMenuStack.emplace(mNextMenu.release());

	mState = mMenuStack.top()->Update(plm);

	switch (mState)
	{
	case MenuBase::ITEM:   if (!mNextMenu) mNextMenu = std::make_unique<MenuItem>(plm); break;
	case MenuBase::STATUS: break; //if (!mNextMenu) mNextMenu = std::make_unique<MenuStatus>(plm);
	}


	// メニュー画面を一つ戻す
	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		// スタックが2個以上ならポップ、1個ならフィールドに戻る
		if (mMenuStack.size() >= 2) mMenuStack.pop();
		else
		{
			if (Fade::GetInstance().SetSceneImage(Fade::SPEED_FAST))
			{
				SceneManager::GetInstance().PopCurrentScene();
			}
		}
	}
}

void MenuManager::Render()
{
	mBG->Render(Vector2::Zero(), Vector2::One(), Vector2::Zero(), mBG->GetSize());
	mMenuStack.top()->Render();
}

void MenuManager::Release()
{
	mMenuSelect.Release();
}
