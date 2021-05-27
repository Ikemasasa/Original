#include "MenuManager.h"

#include "lib/Texture.h"

MenuManager::MenuManager()
{
	mBG = std::make_unique<Texture>(L"Data/Image/Menu/Menu_BG.png");

	mState = MenuSelect::SELECT_MENU;
}

void MenuManager::Initialize()
{
}

void MenuManager::Update(PlayerManager* plm)
{
	switch (mState)
	{
	case MenuSelect::SELECT_MENU:
		mState = mMenuSelect.Update();
		break;

	case MenuSelect::SELECT_ITEM:
		if (!mMenuItem) mMenuItem = std::make_unique<MenuItem>(plm);
		mMenuItem->Update();
		break;

	case MenuSelect::SELECT_STATUS:
		break;
	}
}

void MenuManager::Render()
{
	mBG->Render(Vector2::Zero(), Vector2::One(), Vector2::Zero(), mBG->GetSize());
	switch (mState)
	{
	case MenuSelect::SELECT_MENU: mMenuSelect.Render();	break;
	case MenuSelect::SELECT_ITEM: if(mMenuItem) mMenuItem->Render(); break;
	case MenuSelect::SELECT_STATUS:	break;
	}

}

void MenuManager::Release()
{
	mMenuSelect.Release();
}
