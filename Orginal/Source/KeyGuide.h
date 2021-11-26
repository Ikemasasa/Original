#pragma once
#include <memory>
#include <string>
#include <list>

#include "lib/Font.h"
#include "lib/Sprite.h"

#include "Define.h"

/*
	�L�[�̓����\������N���X
*/
class KeyGuide final
{
	// ���ԍ쐬�֎~
	KeyGuide();
	~KeyGuide() = default;

public:
	enum Key
	{
		A,
		B,
		X,
		Y,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		DPAD,
		LB,
		RB,
	};

private: // �萔

	// �A�C�R���֌W
	static constexpr float ICON_SIZE = 128.0f;						 // �L�[�̃A�C�R���̎��T�C�Y
	static constexpr float ICON_SCALE = 0.25;						 // �L�[�̃A�C�R���̃X�P�[��
	static constexpr float ICON_SCALE_SIZE = ICON_SIZE * ICON_SCALE; // �L�[�̃A�C�R���̃X�P�[���𔽉f�������T�C�Y
	static constexpr float GUIDE_LEFTTOP_X = 12.0f;					 // ��ԍ��̃A�C�R���̃I�t�Z�b�gX
	static constexpr float GUIDE_LEFTTOP_Y = 5.0f;					 // ��ԍ��̃A�C�R���̃I�t�Z�b�gY
	static constexpr float BOARD_LEFTTOP_Y = Define::SCREEN_HEIGHT - ICON_SCALE_SIZE * 2.0f; // �L�[�̃A�C�R�����\�������摜��Y���W

	// ���`�⊮�̌W��
	static constexpr float LERP_FACTOR = 0.1f;

	struct Guide
	{
		std::vector<Key> keys;
		std::wstring description;
	};

private: // �ϐ�

	// �摜
	std::unique_ptr<Sprite> mKeyIcons;		// �L�[�̃A�C�R��
	std::unique_ptr<Sprite> mKeyGuideBoard; // �L�[�̃A�C�R�����\�������摜
	
	// �L�[�K�C�h�֌W
	std::list<Guide> mKeyGuides;		 // �\������L�[�K�C�h�̃��X�g
	Font mFont;							 // �t�H���g
	float mMoveX = Define::SCREEN_WIDTH; // �����ɓ�����


private: // �֐�

	// ���Enum���ɕ\�����邽�߂ɑ}���ꏊ���擾����֐�
	std::list<Guide>::iterator GetInsertIterator(Key key);

	// �L�[�����łɂ��邩�`�F�b�N
	bool ExistKey(Key key);

public:

	// �L�[�K�C�h�ǉ�
	void Add(Key key, std::wstring description);
	void Add(Key* key, int keyNum, std::wstring description);

	// �`��
	void Render(bool isClear);


	void ResetMoveX() { mMoveX = Define::SCREEN_WIDTH; };


	// �V���O���g��
	static KeyGuide& Instance()
	{
		static KeyGuide inst;
		return inst;
	}
};