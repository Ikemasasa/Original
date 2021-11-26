#pragma once
#include <memory>
#include <stack>

#include "MenuSelect.h"
#include "MenuItem.h"

class Sprite;
class PlayerManager;

// ���j���[��ʂ̃}�l�[�W��
class MenuManager
{
	// �ϐ�
	std::unique_ptr<Sprite> mBG;					  // �w�i�摜
	std::stack<std::unique_ptr<MenuBase>> mMenuStack; // ���j���[��ʂ̃X�^�b�N
	MenuBase::Select mNextState;					  //���̉�ʂ̎��ʎq


public: // �֐�

	// �R���X�g���N�^
	MenuManager();

	// ������
	void Initialize(const PlayerManager* plm);
	
	// �X�V
	void Update(PlayerManager* plm);
	
	// �`��
	void Render();
	
	// �`��
	void Release();
};