#include "MenuManager.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Sprite.h"

#include "Fade.h"
#include "MenuEquipment.h"
#include "MenuItem.h"
#include "MenuStatus.h"
#include "SceneManager.h"

MenuManager::MenuManager()
{
	mBG = std::make_unique<Sprite>(L"Data/Image/Menu/menu_bg.png");
}

void MenuManager::Initialize(const PlayerManager* plm)
{
	mMenuStack.emplace(new MenuSelect);
	mMenuStack.top()->Initialize(plm);

	mNextState = MenuBase::NONE;
}

void MenuManager::Update(PlayerManager* plm)
{
	// 効果音
	if (Input::GetButtonTrigger(0, Input::BUTTON::A)) AUDIO.SoundPlay((int)Sound::SELECT);
	if (Input::GetButtonTrigger(0, Input::BUTTON::B)) AUDIO.SoundPlay((int)Sound::CANCEL);

	// ステートによって次のメニューを決める
	switch (mNextState)
	{
	case MenuBase::ITEM:       if (!mNextMenu) mNextMenu = std::make_unique<MenuItem>();   break;
	case MenuBase::EQUIPMENTS: if (!mNextMenu) mNextMenu = std::make_unique<MenuEquipment>(); break;
	case MenuBase::STATUS:     if (!mNextMenu) mNextMenu = std::make_unique<MenuStatus>(); break;

	case MenuBase::BACK:
		mMenuStack.top()->Release();
		mMenuStack.pop();
		break;
	}

	int oldSelectIndex = mMenuStack.top()->mSelectIndex;

	// 次のメニューがあれば更新する
	if (mNextMenu)
	{
		mMenuStack.emplace(mNextMenu.release()); // スタック
		mMenuStack.top()->Initialize(plm);
	}
	mNextState = mMenuStack.top()->Update(plm);

	if (oldSelectIndex != mMenuStack.top()->mSelectIndex) AUDIO.SoundPlay((int)Sound::CURSOR_MOVE);
}

void MenuManager::Render()
{
	mBG->Render(Vector2::ZERO, Vector2::ONE, Vector2::ZERO, mBG->GetSize());
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