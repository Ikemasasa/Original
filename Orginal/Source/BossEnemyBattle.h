#pragma once
#include "BattleCharacter.h"

class BattleCharacterManager;
class Enemy;

// �{�X�̐퓬�p�L�����N�^�[�N���X
class BossEnemyBattle : public BattleCharacter
{

public: // �֐�

	// �R���X�g���N�^
	BossEnemyBattle(const Enemy* enemy);

	// �f�X�g���N�^
	~BossEnemyBattle();

	// ������
	void Initialize() override;

};