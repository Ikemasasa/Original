#pragma once
#include "MenuBase.h"

#include "lib/Font.h"

#include "MenuCharacterSelect.h"

/*
	���j���[�̃X�e�[�^�X�\�����
*/
class MenuStatus : public MenuBase
{
	// �萔
	static constexpr float BOARD_X = 512.0f;			 // �X�e�[�^�X��\������{�[�h��X���W
	static constexpr float BOARD_Y = 128.0f;			 // �X�e�[�^�X��\������{�[�h��Y���W
	static constexpr float CHARA_BOARD_OFFSET_X = 10.0f; // �L�����N�^�[�I���{�[�h�̃I�t�Z�b�gX

	// �e��p�����[�^�̃t�H���g�̃I�t�Z�b�g
	static constexpr float NAME_OFFSET_Y = 10.0f;  
	static constexpr float STATUS_OFFSET_X = 20.0f;
	static constexpr float CUR_OFFSET_X = 96.0f;
	static constexpr float DELIM_OFFSET_X = 4.0f;
	
	static constexpr float STATUS_OFFSET_Y = 60.0f;
	static constexpr float STATUS_ADD_Y = 34.0f;

private: // �ϐ�
	enum StatusKind
	{
		NAME,
		HP,
		MP,
		ATK,
		DEF,
		SPD,
		STATUS_NUM
	};

	std::unique_ptr<Sprite> mStatusPlate; // �X�e�[�^�X�̃{�[�h�̉摜
	MenuCharacterSelect mCharacterSelect; // �L�����N�^�[�I���N���X
	Font mFont; // �t�H���g�N���X

public: // �֐�

	// �R���X�g���N�^
	MenuStatus() = default;

	// �f�X�g���N�^
	~MenuStatus() = default;

	// ������
	void Initialize(const PlayerManager* plm) override;
	
	// �X�V
	Select Update(PlayerManager* plm) override;
	
	// �`��
	void Render() override;
	
	// ���
	void Release() override;
};