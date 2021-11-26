#pragma once

#include "Character.h"
#include "Item.h"


/*
	�t�B�[���h�̃v���C���[�L�����N�^�[�N���X
	��ɓ����̏��������Ă���
*/
class Player : public Character
{
	// �萔
	static constexpr float RUN_SPEED = 0.28f;		  // ����̃X�s�[�h
	static constexpr float RAY_DOWN_DIST = 0.5f;	  // �������̃��C�̋���
	static constexpr float POS_Y_LERP_FACTOR = 0.25f; // Y���W�̕␳�̌W��
	static const int MASS = 100;		 // �v���C���[�̎���

private: // �ϐ�
	ItemInventory mInventory; // �A�C�e���̃C���x���g��

public: // �֐�
	
	// �R���X�g���N�^
	Player(int charaID);

	// �f�X�g���N�^
	~Player() = default;

	// ������
	void Initialize();

	// �X�V
	void Update();

	// �A�C�e���̃C���x���g�����擾
	ItemInventory* GetInventory() { return &mInventory; }

	// �A�C�e���̃C���x���g����ݒ�
	void SetInventory(const ItemInventory* inventory) { mInventory = *inventory; }
};