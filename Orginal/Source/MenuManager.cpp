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
}

void MenuManager::Initialize(const PlayerManager* plm)
{
	mMenuStack.emplace(new MenuSelect);
	mMenuStack.top()->Initialize(plm);

	mNextState = MenuBase::NONE;
}

void MenuManager::Update(const PlayerManager* plm)
{
	// 次のメニューがあれば更新する
	if (mNextMenu)
	{
		mMenuStack.emplace(mNextMenu.release()); // スタック
		mMenuStack.top()->Initialize(plm);
	}
	mNextState = mMenuStack.top()->Update(plm);

	switch (mNextState)
	{
	case MenuBase::ITEM:   if (!mNextMenu) mNextMenu = std::make_unique<MenuItem>();   break;
	case MenuBase::STATUS: if (!mNextMenu) mNextMenu = std::make_unique<MenuStatus>(); break;

	case MenuBase::BACK:
		mMenuStack.top()->Release();
		mMenuStack.pop();
		break;
	}
}

void MenuManager::Render()
{
	mBG->Render(Vector2::Zero(), Vector2::One(), Vector2::Zero(), mBG->GetSize());
	mMenuStack.top()->Render();
}

void MenuManager::Release()
{
	size_t size = mMenuStack.size();
	for (size_t i = 0; i < size; ++i)
	{
		mMenuStack.top()->Release();
		mMenuStack.pop();
	}
}