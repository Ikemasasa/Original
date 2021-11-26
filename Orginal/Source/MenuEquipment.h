#pragma once
#include "MenuBase.h"

#include "lib/Font.h"

#include "EquipmentSelect.h"
#include "MenuCharacterSelect.h"

class Sprite;

/*
	���j���[�̑����i���
	��ɑ��������葕�����O������
*/
class MenuEquipment : public MenuBase
{
	// �萔
	static constexpr float FONT_OFFSET_X = 10.0f; // �����i�^�C�v�̃e�L�X�g�̃I�t�Z�b�gX
	static constexpr float FONT_OFFSET_Y = 10.0f; // �����i�^�C�v�̃e�L�X�g�̃I�t�Z�b�gY
	static constexpr float FONT_ADD_Y    = 32.0f; // 2�s�ڈȍ~��Y���W�ɉ��Z����l

	static constexpr float BOARD_POS_X = 150.0f; // �����i�̃^�C�v��\������{�[�h��X���W
	static constexpr float BOARD_POS_Y = 150.0f;// �����i�̃^�C�v��\������{�[�h��Y���W

private: // �ϐ�
	std::unique_ptr<Sprite> mTypeBoard;	 // �����i�^�C�v��\������{�[�h�̉摜
	std::unique_ptr<Sprite> mTypeSelect; // �����i�^�C�v��I������摜
	MenuCharacterSelect mCharacterSelect; // �L�����N�^�[�I���N���X
	EquipmentSelect mEquipmentSelect;	  // �����i�I���N���X
	Font mFont; // �t�H���g�N���X
	bool mIsDecideType = false; // �^�C�v�����܂�����
	int mOldSelectIndex = 0; // �O��̑I���ꏊ

public:// �֐�
	
	// ������
	void Initialize(const PlayerManager* plm) override;
	
	// �X�V
	Select Update(PlayerManager* plm) override;
	
	// �`��
	void Render() override;

	// ���
	void Release() override;
};