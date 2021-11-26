#pragma once
#include "IDecideCommand.h"

#include "ItemSelect.h"

/*
	�퓬���̃R�}���h�ŃA�C�e����I�񂾌�̏���������N���X
	�����Ă�A�C�e���̒�����I������
*/
class DecideItem : public IDecideCommand
{
	ItemSelect mItemSelect; // �A�C�e���I���N���X

public: // �֐�

	// �R���X�g���N�^
	DecideItem() = default;

	// �f�X�g���N�^
	~DecideItem() = default;

	// ������
	void Initialize(const BattleCharacterManager* bcm) override;

	// �X�V
	void Update(const BattleCharacterManager* bcm, CommandBase* cmdBase) override;

	// �`��
	void Render();
};