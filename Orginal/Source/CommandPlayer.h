#pragma once
#include <memory>
#include <stack>

#include "CommandBase.h"

class IDecideCommand;
class Sprite;

// �v���C���[�̃R�}���h����N���X
class CommandPlayer : public CommandBase
{
private: // �ϐ�
	std::stack<std::unique_ptr<IDecideCommand>> mCommand; // �R�}���h�I���̃X�^�b�N

public: // �֐�

	// �R���X�g���N�^
	CommandPlayer();

	// �f�X�g���N�^
	~CommandPlayer();

	// �X�V
	void Update(const BattleCharacterManager* bcm) override;
	
	// �`��
	void Render() const override;

	// �s���I��
	void BehaviourFinished() override;
};