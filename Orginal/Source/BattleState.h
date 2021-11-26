#pragma once
#include "Singleton.h"

class CameraBase;

/*
	�o�g����State�ݒ�N���X(�v���P
	��ɃJ�����p
*/
class BattleState : public SingletonManual<BattleState>
{
public:// �萔
	static constexpr float BEGIN_TIME = 1.0f; // �J�n���o�̎���

	enum class State
	{
		BEGIN,
		BEGIN_FINISHED,
		COMMAND_SELECT,
		PARTY_SELECT,
		ENEMY_SELECT,
		PLAYER_ATTACK,
		ENEMY_ATTACK,
		ESCAPE,
		RESULT,
		MAX
	};

private: // �ϐ�
	State mState = State::BEGIN;	// ���݂̃X�e�[�g
	State mOldState = State::BEGIN; // �O��̃X�e�[�g

public: // �֐�

	// �R���X�g���N�^
	BattleState() = default;

	// �f�X�g���N�^
	~BattleState() = default;

	void SetState(State state) { mOldState = mState; mState = state; }; // �X�e�[�g��ݒ�
	State GetState() const { return mState; }							// �X�e�[�g���擾
	State GetOldState() const { return mOldState; }						// �O��̃X�e�[�g���擾
	bool CheckState(State state) { return mState == state; }			// �����̃X�e�[�g�Ɠ������`�F�b�N
	bool CheckOldState(State state) { return mOldState == state; }		// �����̃X�e�[�g�ƑO��̃X�e�[�g���������`�F�b�N

};