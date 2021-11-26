#pragma once
#include "CommandBase.h"

class BattleCharacterManager;

// �{�X�̃R�}���h����N���X
class CommandBossEnemy : public CommandBase
{
	// �萔
	static constexpr float ATTACK_LOWHP_CHARA_PERCENT = 30.0f; // ��HP�̃L������_���m��

private: // �ϐ�
	enum class State
	{
		TARGET_ONE,
		TARGET_TWO,
		
		MAX
	};
	State mState = State::TARGET_ONE; // �^�[�Q�b�g�̐�


private: // �֐�

	// lowHP�̃v���C���[��CharaID��Ԃ�
	std::vector<int> SearchLowHPPlayer(const size_t num, const BattleCharacterManager* bcm);

	// �����_���̃v���C���[ID���擾
	std::vector<int> GetRandomPlayerObjIDs(const size_t num, const BattleCharacterManager* bcm);

public: 
	
	// �R���X�g���N�^
	CommandBossEnemy();

	//�f�X�g���N�^
	~CommandBossEnemy();

	// �X�V
	void Update(const BattleCharacterManager* bcm) override;
};