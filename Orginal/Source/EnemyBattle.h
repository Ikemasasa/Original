#pragma once
#include "BattleCharacter.h"

class Enemy;

// �G�̐퓬�p�L�����N�^�[�N���X
class EnemyBattle : public BattleCharacter
{

public: // �֐�

	// �R���X�g���N�^
	EnemyBattle(const Enemy* enemy);

	// �f�X�g���N�^
	~EnemyBattle() = default;

	// ������
	void Initialize() override;
};