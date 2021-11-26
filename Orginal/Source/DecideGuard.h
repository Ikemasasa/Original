#pragma once
#include "IDecideCommand.h"

#include "SelectOptions.h"


/*
	�퓬���̃R�}���h�œ������I�񂾌�̏���������N���X
	 ���g�I�ɂ́u�{���ɃK�[�h�����܂����H�v�݂����Ȃ��Ƃ����Ă邾��
*/
class DecideGuard : public IDecideCommand
{
	// �萔
	static constexpr float BOARD_POS_X = 200.0f; // �I�����̉摜��X���W
	static constexpr float BOARD_POS_Y = 220.0f; // �I�����̉摜��Y���W
	SelectOptions mSelectOptions;				 // �I������I�ԃN���X


public: // �֐�

	// �R���X�g���N�^
	DecideGuard() = default;

	// �f�X�g���N�^
	~DecideGuard() = default;

	// ������
	void Initialize(const BattleCharacterManager* bcm) override;

	// �X�V
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;

	// �`��
	void Render() override;
};