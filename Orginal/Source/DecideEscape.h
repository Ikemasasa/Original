#pragma once
#include <memory>

#include "Define.h"
#include "IDecideCommand.h"
#include "SelectOptions.h"

/*
	�퓬���̃R�}���h�œ������I�񂾌�̏���������N���X
	 ���g�I�ɂ́u�{���ɓ����܂����H�v�݂����Ȃ��Ƃ����Ă邾��
*/
class DecideEscape : public IDecideCommand
{
	// �萔
	static constexpr float BOARD_POS_X = 200.0f; // �I�����̉摜��X���W
	static constexpr float BOARD_POS_Y = 220.0f; // �I�����̉摜��Y���W
	SelectOptions mSelectOptions;				 // �I������I�ԃN���X

public: // �֐�

	// �R���X�g���N�^
	DecideEscape() = default;

	// �f�X�g���N�^
	~DecideEscape() = default;

	// ������
	void Initialize(const BattleCharacterManager* bcm) override;

	// �X�V
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;
	
	// �`��
	void Render() override;
};