#pragma once
#include <memory>
#include <vector>

#include "lib/Font.h"

#include "EquipmentInventory.h"

class Sprite;


// �����I���N���X
class EquipmentSelect
{
	// ����
	static const int HORIZONTAL_NUM = 5; // �A�C�R������̐�
	static const int STATUS_NUM = 6;     // �A�C�R���c��̐�
	static const int PARAM_NUM = 3;		 // �����̃p�����[�^�̐�

	static constexpr float ICON_OFFSET = 10.0f; // �A�C�R��
	static constexpr float ICON_SIZE = 256.0f;	// �A�C�R���̎��T�C�Y
	static constexpr float ICON_SCALE = 0.25f;  // �A�C�R���̃X�P�[��

	static constexpr float INFO_BOARD_OFFSET_X = 400.0f; // �x�[�X�{�[�h����ڍׂ̃{�[�h�܂ł̃I�t�Z�b�gX
	static constexpr float INFO_ICON_OFFSET_X = 114.0f;  // �ڍׂ̃{�[�h����A�C�R���\���ꏊ�I�t�Z�b�gX
	static constexpr float INFO_ICON_OFFSET_Y = 76.0f;   // �ڍׂ̃{�[�h����A�C�R���\���ꏊ�I�t�Z�b�gY
	static constexpr float INFO_ICON_SCALE = 0.5f;		 // �ڍ׃{�[�h�̃A�C�R���̃X�P�[��

private: // �ϐ�
	std::unique_ptr<Sprite> mBoard;			// �x�[�X�̃{�[�h�摜
	std::unique_ptr<Sprite> mSelectFrame;	// �I�������ꏊ�ɕ\������摜
	std::unique_ptr<Sprite> mInfoBoard;		// �ڍׂ̃{�[�h
	std::unique_ptr<Sprite> mEquipmentMark; // �������̃}�[�N�摜
	int mSelectIndex = 0;					// �I�𒆂̏ꏊ
	std::vector<EquipmentInventory::Data> mEquipmentsData; // �^�C�v�ʂ̑����i���X�g

	Font mFont;						 // �t�H���g�N���X
	std::vector<std::wstring> mInfo; // �����i�̐����̃e�L�X�g

private: // �֐�

	 // �����i�̐����̃e�L�X�g��ݒ�
	void RenderSetInfo(const Vector2& infoBoardPos);

public:

	// ������
	void Initialize();

	// �X�V
	int Update(const std::vector<EquipmentInventory::Data>& equipmentsData);

	// �`��
	void Render(const Vector2& boardPos);

	// �����i���X�g
	void ClearData();
};