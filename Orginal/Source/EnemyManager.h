#pragma once
#include <list>

#include "lib/Vector.h"

#include "DataBase.h"

class Enemy;
class Shader;

//
/*
	�G�l�~�[�}�l�[�W���N���X
	�G�̐����A�X�V�A�`������Ă���
*/
class EnemyManager
{
public:
	enum EnmList
	{
		MONSTER_A = DataBase::ENM_ID_START,
		MONSTER_B,
		BOSS_MONSTER,

		ENM_NUM
	};

private: // �ϐ�
	std::list<std::unique_ptr<Enemy>> mEnemies; // �G���X�g
	Vector3 mPlayerPos = {};					// �v���C���[���W

public: // �֐�

	// �R���X�g���N�^
	EnemyManager();

	// �f�X�g���N�^
	~EnemyManager();

	// �G�𐶐�
	void Create(int charaID); 

	// �G���폜
	std::list<std::unique_ptr<Enemy>>::iterator Destroy(int objID);

	// ������
	void Initialize();

	// �X�V
	void Update(const bool isTalking);
	
	// �`��
	void Render(const Shader* shader, const Matrix& view, const Matrix& projection, const Vector4& lightDir);

	// �G�̐����擾
	int GetNum() const { return mEnemies.size(); }
	
	// ������ID�̓G���擾
	Enemy* GetEnemy(int objID);

	// �v���C���[���W��ݒ�
	void SetPlayerPos(const Vector3& pos) { mPlayerPos = pos; }
};