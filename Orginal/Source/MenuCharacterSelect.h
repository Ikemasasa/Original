#pragma once
#include <memory>

#include "lib/Font.h"

class PlayerManager;
class Sprite;

/*
	�Q�Ƃ���L�����N�^�[�����߂�N���X
	�N�̃C���x���g�������邩�A�Ƃ�
	�N�̃X�e�[�^�X�����邩�A�Ƃ��Ŏg���Ă�
*/
class MenuCharacterSelect
{
	// �萔
	static constexpr float FONT_OFFSET_X = 2;  // �摜����t�H���g�`��ʒu�܂ł̋���
	static constexpr float FONT_OFFSET_Y = 32; // �t�H���g�T�C�Y / 2

private: // �ϐ�
	std::unique_ptr<Sprite> mBoard;			// ���n�̉摜
	int mSelectIndex = 0;					// �I���ꏊ
	int mOldSelectIndex = 0;				// 1�t���[���O�̑I���ꏊ
	std::vector<wchar_t> mPlayerNameFirst;	// �L�����N�^�[��1�����ڂ̃��X�g
	Font mFont;							    // �t�H���g�N���X

public: // �֐�

	// ������
	void Initialize(const PlayerManager* plm);

	// �X�V
	void Update();

	// �`��
	void Render(Vector2 leftBottom);

	// ���
	void Release();

	// 1�t���[���O����I���ꏊ���ς������
	bool IsChangedIndex()const { return mSelectIndex != mOldSelectIndex; }

	// ���݂̑I���ꏊ���擾
	int GetIndex() const { return mSelectIndex; }
};