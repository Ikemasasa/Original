#pragma once
#include "IDecideCommand.h"

#include <vector>

#include "SkillSelect.h"

/*
	�퓬���̃R�}���h�ŃX�L����I�񂾌�̏���������N���X
	 �g���X�L����I������
*/
class DecideSkill : public IDecideCommand
{
	// �萔
	static constexpr float BOARD_POS_X = 300; // �X�L�����X�g��\������{�[�h��X���W
	static constexpr float BOARD_POS_Y = 200; // �X�L�����X�g��\������{�[�h��Y���W
	SkillSelect mSkillSelect;			      // �X�L���I���N���X

public: // �֐�

	// �R���X�g���N�^
	DecideSkill() = default;

	// �f�X�g���N�^
	~DecideSkill() = default;

	// ������
	void Initialize(const BattleCharacterManager* bcm) override;

	// �X�V
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;

	// �`��
	void Render();
};