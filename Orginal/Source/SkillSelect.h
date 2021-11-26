#pragma once
#include <memory>
#include <vector>

#include "lib/Font.h"

#include "SkillData.h"

class Sprite;

/*
	�X�L���I���N���X
*/
class SkillSelect
{
	// �萔
	static constexpr float SKILLNAME_POS_X = 18.0f;	 // �X�L�����̃e�L�X�g��X���W
	static constexpr float SKILLNAME_POS_Y = 18.0f;  // �X�L�����̃e�L�X�g��Y���W
	static constexpr float USE_MP_OFFSET_X = 280.0f; // MP�g�p�ʂ̃e�L�X�g��X���W
	static const int VERTICAL_SKILL_MAX = 6;		 // �c�ɕ��ׂ���X�L���̐�
	static const int FONT_SIZE = 32;				 // �t�H���g�T�C�Y

	static constexpr float SKILLINFO_POS_X = 14.0f;	 // �X�L���̐������̃e�L�X�g��X���W
	static constexpr float SKILLINFO_POS_Y = 229.0f; // �X�L���̐������̃e�L�X�g��Y���W
	static constexpr float INFO_LINE_PX = 420.0f;	 // �X�L���̐������̃e�L�X�g�̉����̍ő�px

private: // �ϐ�
	std::unique_ptr<Sprite> mSkillBoard;				// �X�L���ꗗ�̃{�[�h�̉摜
	std::unique_ptr<Sprite> mSkillSelect;				// �X�L���I���̉摜
	std::vector <const SkillData::BaseData*> mSkills;   // �\������X�L���̃��X�g
	int mSkillIndex = 0;								// �I���ꏊ
	Vector2 mBoardPos;								    // �{�[�h�̍��W
	Font mFont;											// �t�H���g�N���X

public: // �֐�

	// ������
	void Initialize(const int charaID, const Vector2& leftTop);

	// �X�V
	void Update();

	// �`��
	void Render();

	// �I�񂾃X�L���̃p�����[�^���擾
	const SkillData::BaseData* GetSelectSkill() const { return mSkills[mSkillIndex]; }

	// ���ݑI��ł�I���ꏊ���擾
	int GetIndex() const { return mSkillIndex; }

	// �g�p�\�ȃX�L�������邩
	bool ExistSkill() const { return !mSkills.empty(); }
};