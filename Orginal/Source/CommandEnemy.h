#pragma once
#include "CommandBase.h"

// ���ʂ̓G�̃R�}���h����N���X
class CommandEnemy : public CommandBase
{
public:

	// �R���X�g���N�^
	CommandEnemy() = default;

	// �f�X�g���N�^
	~CommandEnemy() = default;

	// �X�V
	void Update(const BattleCharacterManager* bcm) override;
};