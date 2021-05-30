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
	// ���̃��j���[������΍X�V����
	if (mNextMenu) mMenuStack.emplace(mNextMenu.release());

	mState = mMenuStack.top()->Update(plm);

	switch (mState)
	{
	case MenuBase::ITEM:   if (!mNextMenu) mNextMenu = std::make_unique<MenuItem>(plm); break;
	case MenuBase::STATUS: break; //if (!mNextMenu) mNextMenu = std::make_unique<MenuStatus>(plm);
	}


	// ���j���[��ʂ���߂�
	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		// �X�^�b�N��2�ȏ�Ȃ�|�b�v�A1�Ȃ�t�B�[���h�ɖ߂�
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
