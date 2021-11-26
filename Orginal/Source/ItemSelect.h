#pragma once
#include "lib/Font.h"
#include "lib/Vector.h"

#include "Item.h"

class Sprite;

/*
	����A�C�e����I�ԃN���X
	�g���܂킹���������A1�̃N���X�ɏ����Ɨʂ�������������������N���X��
*/
class ItemSelect
{
public: // �萔
	static constexpr float ICON_OFFSET = 10.0f; // �{�[�h�̍��ォ��A�C�R���`��ʒu�܂ł̋���(XY����)

private:

	// �A�C�R���֌W
	static const int HORIZONTAL_NUM = 5; // �A�C�R������̐�
	static const int VERTICAL_NUM = 6;   // �A�C�R���c��̐�
	static constexpr float ICON_SIZE = 256.0f; // �A�C�R���̎��T�C�Y
	static constexpr float ICON_SCALE = 0.25f; // �A�C�R���̃X�P�[��
		
	// �A�C�e�����֌W
	static constexpr float INFO_BOARD_OFFSET_X = 512.0f; // �A�C�e���I���摜����̃I�t�Z�b�gX
	static constexpr float INFO_ICON_OFFSET_X = 114.0f;  // ���摜�ɕ\�������A�C�R���̃I�t�Z�b�gX
	static constexpr float INFO_ICON_OFFSET_Y = 76.0f;   // ���摜�ɕ\�������A�C�R���̃I�t�Z�b�gY
	static constexpr float INFO_ICON_SCALE = 0.5f;		 // ���摜�ɕ\�������A�C�R���̃X�P�[��
	static constexpr float INFO_TEXT_OFFSET_X = 25;		 // ���摜�ɕ\�������e�L�X�g�̃I�t�Z�b�gX
	static constexpr float INFO_TEXT_OFFSET_Y = 255;	 // ���摜�ɕ\�������e�L�X�g�̃I�t�Z�b�gY
	static constexpr float INFO_TEXT_ADD_Y = 45;		 // 2�s�ڈȍ~�̃e�L�X�g�̃I�t�Z�b�gY
	static constexpr float INFO_WIDTH_MAX = 300;		 // �e�L�X�g�̍ő剡��
	static const int INFO_LINE_MAX = 3;					 // �e�L�X�g�̍ő�s��

private: // �ϐ�
	
	// �摜�֌W
	std::unique_ptr<Sprite> mItemBoard;	  // �A�C�R�����\�������摜
	std::unique_ptr<Sprite> mSelectFrame; // �I�����Ă���ꏊ�ɕ\�������摜
	std::unique_ptr<Sprite> mInfoBoard;   // �I�����Ă���A�C�e���̏ڍׂ��\�������摜

	// �C���x���g���֌W
	const ItemInventory* mInventoryRef = nullptr; // �Q�Ƃ��Ă���C���x���g��
	int mSelectIndex  = 0;						  // ���ݑI�����Ă���ꏊ

	// ���֌W
	Font mFont;						 // �t�H���g
	std::vector<std::wstring> mInfo; // �A�C�e�����̃e�L�X�g

private: // �֐�

	// �A�C�e�������X�V
	void UpdateInfo();

	// �A�C�e�����摜�ɕ\�������t�H���g�̐ݒ�
	void RenderSetInfo(const Vector2& infoBoardPos);

public:
	
	// �R���X�g���N�^
	ItemSelect() = default;

	// �f�X�g���N�^
	~ItemSelect() = default;

	// �������֐�
	void Initialize();

	// �X�V�֐�
	void Update(const ItemInventory* inventory);

	// �`��֐�
	void Render(const Vector2& boardOffset);

	int GetIndex() const { return mSelectIndex; } // ���ݑI�����Ă���ꏊ���擾
	Vector2 GetCursorRightUpPos() const;		  // �{�[�h�̈ʒu���܂܂Ȃ�(0, 0)�
};