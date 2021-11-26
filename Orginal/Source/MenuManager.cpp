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
	// �摜�ǂݍ���
	mBG = std::make_unique<Sprite>(L"Data/Image/Menu/menu_bg.png");
}

void MenuManager::Initialize(const PlayerManager* plm)
{
	// ��ԍŏ��̃X�^�b�N�Ƀ��j���[�Z���N�g��ݒ�
	mMenuStack.emplace(new MenuSelect);
	mMenuStack.top()->Initialize(plm);

	mNextState = MenuBase::NONE;
}

void MenuManager::Update(PlayerManager* plm)
{
	// ���ʉ�
	if (Input::GetButtonTrigger(0, Input::BUTTON::A)) Sound::Play(Sound::SELECT);
	if (Input::GetButtonTrigger(0, Input::BUTTON::B)) Sound::Play(Sound::CANCEL);

	// �X�e�[�g�ɂ���Ď��̃��j���[�����߂�
	std::unique_ptr<MenuBase> next = nullptr; // ���̃��j���[���
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

	// ���̃��j���[������΍X�V����
	if (next)
	{
		mMenuStack.emplace(next.release()); // �X�^�b�N
		mMenuStack.top()->Initialize(plm);
	}
	mNextState = mMenuStack.top()->Update(plm);

	if (oldSelectIndex != mMenuStack.top()->mSelectIndex) Sound::Play(Sound::CURSOR_MOVE);
}

void MenuManager::Render()
{
	// �w�i�`��
	mBG->Render(Vector2::ZERO, Vector2::ONE, Vector2::ZERO, mBG->GetSize());
	
	// ���j���[��ʂ̕`��
	mMenuStack.top()->Render();
}

void MenuManager::Release()
{
	// �X�^�b�N�S���
	size_t size = mMenuStack.size();
	for (size_t i = 0; i < size; ++i)
	{
		mMenuStack.top()->Release();
		mMenuStack.pop();
	}
}