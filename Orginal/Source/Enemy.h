#pragma once
#include <memory>
#include <DirectXMath.h>

#include "Character.h"
#include "StatusData.h"

// �t�B�[���h�̃{�X�L�����N�^�[�N���X
class Enemy : public Character
{
	// �萔
	static constexpr float WAIT_TO_WALK_SEC = 7.0f; // �ҋ@��������܂ł̊Ԋu
	static constexpr float WALK_TO_WAIT_SEC = 3.0f; // ��������ҋ@�܂ł̊Ԋu
	static constexpr float WALK_SPEED = 0.08f;		// �����̃X�s�[�h
	static constexpr float RUN_SPEED = WALK_SPEED * 2.0f; // ����̃X�s�[�h
	static const int MASS = 0;	     // �G�̎���
private: // �ϐ�
	enum MoveState
	{
		WAIT,
		WALK,
		CHASE,

		MAX
	};

	MoveState mState = WAIT;		// �����̃X�e�[�g
	StatusData::EnemyType mEnmType; // �G�̃^�C�v
	float mTimer = 0;				// �^�C�}�[
	float mMoveAngle;				// �����̎��̊p�x

private: // �֐�
	
	// �v���C���[��ǂ�������X�e�[�g�ɓ��邩�`�F�b�N
	bool IsChase(const Vector3& playerPos);

	// �ړ��X�V
	void Move(const Vector3& playerPos);
	
	// �ҋ@�X�e�[�g
	void StateWait();

	// �����X�e�[�g
	void StateWalk();

	// �ǂ������X�e�[�g
	void StateChase(const Vector3& playerPos);
public:

	// �R���X�g���N�^
	Enemy(int charaID);

	// �f�X�g���N�^
	virtual ~Enemy();

	// ������
	virtual void Initialize();

	// �X�V
	virtual void Update(const Vector3& playerPos);
	
	// �G�̃^�C�v���擾
	StatusData::EnemyType GetEnmType() const { return mEnmType; }
	
	// �G�̃^�C�v��ݒ�
	void SetEnmType(const StatusData::EnemyType& type) { mEnmType = type; }
};