#include "MenuManager.h"

#include "lib/Input.h"
#include "lib/Sprite.h"

#include "MenuEquipment.h"
#include "MenuItem.h"
#include "MenuStatus.h"
#include "SceneManager.h"
#include "Sound.h"

MenuManager::MenuManager()
{
	// 画像読み込み
	mBG = std::make_unique<Sprite>(L"Data/Image/Menu/menu_bg.png");
}

void MenuManager::Initialize(const PlayerManager* plm)
{
	// 一番最初のスタックにメニューセレクトを設定
	mMenuStack.emplace(new MenuSelect);
	mMenuStack.top()->Initialize(plm);

	mNextState = MenuBase::NONE;
}

void MenuManager::Update(PlayerManager* plm)
{
	// 効果音
	if (Input::GetButtonTrigger(0, Input::BUTTON::A)) Sound::Play(Sound::SELECT);
	if (Input::GetButtonTrigger(0, Input::BUTTON::B)) Sound::Play(Sound::CANCEL);

	// ステートによって次のメニューを決める
	std::unique_ptr<MenuBase> next = nullptr; // 次のメニュー画面
	switch (mNextState)
	{
	case MenuBase::ITEM:       next = std::make_unique<MenuItem>();   break;
	case MenuBase::EQUIPMENTS: next = std::make_unique<MenuEquipment>(); break;
	case MenuBase::STATUS:     next = std::make_unique<MenuStatus>(); break;

	case MenuBase::BACK:
		mMenuStack.top()->Release();
		mMenuStack.pop();
		break;
	}

	int oldSelectIndex = mMenuStack.top()->mSelectIndex;

	// 次のメニューがあれば更新する
	if (next)
	{
		mMenuStack.emplace(next.release()); // スタック
		mMenuStack.top()->Initialize(plm);
	}
	mNextState = mMenuStack.top()->Update(plm);

	if (oldSelectIndex != mMenuStack.top()->mSelectIndex) Sound::Play(Sound::CURSOR_MOVE);
}

void MenuManager::Render()
{
	// 背景描画
	mBG->Render(Vector2::ZERO, Vector2::ONE, Vector2::ZERO, mBG->GetSize());
	
	// メニュー画面の描画
	mMenuStack.top()->Render();
}

void MenuManager::Release()
{
	// スタック全解放
	size_t size = mMenuStack.size();
	for (size_t i = 0; i < size; ++i)
	{
		mMenuStack.top()->Release();
		mMenuStack.pop();
	}
}