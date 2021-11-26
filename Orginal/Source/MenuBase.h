#pragma once

#include "lib/Audio.h"
#include "lib/Input.h"

class PlayerManager;

/*
	���j���[��ʂ̊��N���X
	�e���j���[��ʂ̃N���X�Ɍp������
*/
class MenuBase
{
	// �t�����h�N���X
	friend class MenuManager;

protected: // �ϐ�
	enum Select
	{
		ITEM,
		EQUIPMENTS,
		STATUS,
		SELECT_NUM,

		BACK,
		NONE = 10 // �A�C�e��(�X�e�[�^�X�ł���������)����ԏ�ɂ��邽�߂�NONE����ԍŌ�ɂ��Ă�
	};

	int mSelectIndex = 0; // �I���ꏊ

public: // �֐�

	// �R���X�g���N�^
	MenuBase() = default;

	// �f�X�g���N�^
	virtual ~MenuBase() = default;

	// ������
	virtual void Initialize(const PlayerManager* plm) = 0;

	// �X�V
	virtual Select Update(PlayerManager* plm) = 0;
	
	// �`��
	virtual void Render() = 0;
	
	// ���
	virtual void Release() = 0;
};