 #pragma once
#include "Enemy.h"

// �t�B�[���h�̃{�X�L�����N�^�[�N���X
class BossEnemy : public Enemy
{
	// �萔
	static constexpr float MASS = 1000.0f; // ����

public: // �֐�

	// �R���X�g���N�^
	BossEnemy(int charaID);

	// �f�X�g���N�^
	~BossEnemy();

	// ������
	void Initialize() override;

	// �X�V
	void Update(const Vector3& playerPos) override;
};