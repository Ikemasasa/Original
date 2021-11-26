#include "CameraBattle.h"

#include "BattleCharacter.h"
#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CommandBase.h"
#include "Fade.h"
#include "GameManager.h"

CameraBattle::CameraBattle() : CameraBase()
{

}

void CameraBattle::Initialize(const Vector3& target, float distance)
{
	// ���N���X�̏������֐�
	CameraBase::Initialize(target, distance);

	// ��������^�[�Q�b�g�ݒ�
	mTarget = target;

	// �����A�V���p��萔����ݒ�
	mDistance = distance;
	mZenithAngle = BEGIN_START_ZENITH;
	mAzimuthAngle = BEGIN_START_AZIMUTH;

	// ���W��ݒ�
	mPos = CalcPosFromAngle(distance);
	mBeginPos = mPos;
	mEndPos = CalcPosFromAngle(BEGIN_END_DISTANCE);

	// �r���[�X�V
	UpdateView();
}

void CameraBattle::Update(const BattleCharacter* commandChara, const BattleCharacterManager* bcm)
{
	// �U���I���t���O�����Z�b�g
	ResetAttackFinishFlag();

	// BattleState�ɂ���ĕ���
	BattleState::State state = BattleState::GetInstance().GetState();

	switch (state)
	{
	case BattleState::State::BEGIN:			 Begin(commandChara); break;
	case BattleState::State::COMMAND_SELECT: CommandSelect(commandChara); break;
	case BattleState::State::PARTY_SELECT:   PartySelect(commandChara); break;
	case BattleState::State::ENEMY_SELECT:   EnemySelect(commandChara); break;
	case BattleState::State::PLAYER_ATTACK:  PlayerAttack(commandChara, bcm); break;
	case BattleState::State::ENEMY_ATTACK:	 EnemyAttack(commandChara, bcm); break;
	case BattleState::State::ESCAPE:		 Escape(commandChara); break;
	}

	// �U���A�r���[���X�V
	UpdateShake(GameManager::elapsedTime);
	UpdateView();
}

void CameraBattle::Begin(const BattleCharacter* commandChara)
{	
	// �t�F�[�h�A�E�g��a�l��LERP_START_FADE_ALPHA���傫���Ȃ�return
	if (!Fade::GetInstance().CheckFadeState(Fade::FADE_OUT, LERP_START_FADE_ALPHA)) return;

	// ���W����`�⊮�ōX�V
	mPos = Vector3::Lerp(mBeginPos, mEndPos, mLerpFactor);

	// �W����1�����Ȃ�W�������Z����
	if (mLerpFactor < 1.0f)
	{
		mLerpVelocity += BEGIN_LERP_ACCEL;
		mLerpFactor += mLerpVelocity;
	}
	else
	{
		// 1�ȏ�(�ړ����I��)�Ȃ�^�C�}�[�����Z
		mTimer += GameManager::elapsedTime;
	}

	// �^�C�}�[��BattleState::BEGIN_TIME�𒴂�����J�n���o�I��
	if (mTimer >= BattleState::BEGIN_TIME)
	{
		// BattleState���J�n���o�I���ɂ���
		BattleState::GetInstance().SetState(BattleState::State::BEGIN_FINISHED);


		// mCommandSelectPos�ɏ����l������

		// �R�}���h�L�����̏����擾
		Vector3 charaPos = commandChara->GetPos(); 
		float radius = commandChara->GetCapsule().radius;
		float diameter = radius * 2.0f;
		Vector3 front = commandChara->GetFrontXZ();
		Vector3 right = commandChara->GetRightXZ();

		// �L�����̍��W�ɂ���č��E�ɕ�����
		if (charaPos.x < 0) right = -right;

		// ���W����
		mCommandSelectPos.x = charaPos.x + (front.x * diameter * COMMAND_SELECT_DIST_XZ) + (right.x * radius * COMMAND_SELECT_DIST_XZ);
		mCommandSelectPos.y = charaPos.y + commandChara->GetHeight() * COMMAND_SELECT_HEIGHT_RATE;
		mCommandSelectPos.z = charaPos.z + (front.z * diameter * COMMAND_SELECT_DIST_XZ) + (right.z * radius * COMMAND_SELECT_DIST_XZ);
	}
}

void CameraBattle::CommandSelect(const BattleCharacter* commandChara)
{
	// �R�}���h�L�����̏����擾
	Vector3 charaPos = commandChara->GetPos();
	float radius = commandChara->GetCapsule().radius;
	float diameter = radius * 2.0f;
	Vector3 front = commandChara->GetFrontXZ();
	Vector3 right = commandChara->GetRightXZ();

	// �L�����̍��W�ɂ���č��E�ɕ�����
	if (charaPos.x < 0) right = -right;

	// ���W�Z�o
	Vector3 pos = {};
	pos.x = charaPos.x + (front.x * diameter * COMMAND_SELECT_DIST_XZ) + (right.x * radius * COMMAND_SELECT_DIST_XZ);
	pos.y = charaPos.y + commandChara->GetHeight() * COMMAND_SELECT_HEIGHT_RATE;
	pos.z = charaPos.z + (front.z * diameter * COMMAND_SELECT_DIST_XZ) + (right.z * radius * COMMAND_SELECT_DIST_XZ);

	// �R�}���h�Z���N�g���W�ɐݒ�
	mCommandSelectPos = pos;

	// ���W����`�⊮�Őݒ�
	mPos = Vector3::Lerp(mPos, pos, LERP_SPEED);
	
	// �^�[�Q�b�g��G�̍��W�̐^�񒆂ɐݒ�
	Vector3 target = Vector3(0.0f, 0.0f, BattleCharacterManager::ENEMY_POS_Z);
	mTarget = Vector3::Lerp(mTarget, target, LERP_SPEED);
}

void CameraBattle::PartySelect(const BattleCharacter* commandChara)
{
	// ���W���R�}���h�Z���N�g��Z���W���]�ɐݒ�
	mPos = Vector3::Lerp(mPos, Vector3(mCommandSelectPos.x, mCommandSelectPos.y, -mCommandSelectPos.z), LERP_SPEED);

	// �^�[�Q�b�g���v���C���[�̍��W�̐^�񒆂ɐݒ�
	mTarget = Vector3::Lerp(mTarget, Vector3(0.0f, 0.0f, BattleCharacterManager::PLAYER_POS_Z), LERP_SPEED);
}

void CameraBattle::EnemySelect(const BattleCharacter* commandChara)
{
	// ���W���R�}���h�Z���N�g���W�ɐݒ�
	mPos = Vector3::Lerp(mPos, mCommandSelectPos, LERP_SPEED);

	// �^�[�Q�b�g��G�̍��W�̐^�񒆂ɐݒ�
	mTarget = Vector3::Lerp(mTarget, Vector3(0.0f, 0.0f, BattleCharacterManager::ENEMY_POS_Z), LERP_SPEED);
}

void CameraBattle::PlayerAttack(const BattleCharacter* commandChara, const BattleCharacterManager* bcm)
{	
	// �R�}���h�L�����̃��[�V�������擾
	Character::MotionType oldMotion = mCommandCharaMotion;
	mCommandCharaMotion = commandChara->GetMotion();

	// ���[�V�������A�O�񂪍U�����A����U������Ȃ��Ȃ�Attackfinishflag��on
	if (oldMotion == Character::ATTACK && mCommandCharaMotion != Character::ATTACK)
	{
		mPlAttackFinishFlag = true;
	}

	// ���W���Z�o����
	Vector3 pos = {};
	if (mPlAttackFinishFlag)
	{
		// �U�����I����Ă�����R�}���h�I���̍��W
		pos = mCommandSelectPos;
	}
	else
	{
		// �U�����I����Ă��Ȃ�������
		// �v���C���[�̏��
		Vector3 charaPos = commandChara->GetPos();
		float radius = commandChara->GetCapsule().radius;
		float diameter = radius * 2.0f;
		Vector3 front = commandChara->GetFrontXZ();
		Vector3 right = commandChara->GetRightXZ();
		if (charaPos.x < 0) right = -right;

		// ���W���Z�o
		pos.x = charaPos.x + (front.x * radius * ATTACK_DISTANCE) + (right.x * diameter * ATTACK_DISTANCE);
		pos.y = charaPos.y + commandChara->GetHeight() * ATTACK_HEIGHT_RATE;
		pos.z = charaPos.z + (front.z * radius * ATTACK_DISTANCE) + (right.z * diameter * ATTACK_DISTANCE);

	}
	// ���W���X�V
	mPos = Vector3::Lerp(mPos, pos, LERP_SPEED);



	// �^�[�Q�b�g����
	Vector3 target = {  };
	if (commandChara->GetMotion() == Character::ATTACK)
	{
		// �U�����[�V�������Ȃ�A�U������̕��ύ��W���^�[�Q�b�g��

		// ID�擾
		std::vector<int> targetIds = commandChara->GetCommand()->GetTargetObjIDs();

		// ���ς̍��W�����߂�
		for (const auto& id : targetIds)
		{
			BattleCharacter* bc = bcm->GetChara(id);
			target += bc->GetTargetPos();
		}
		target /= targetIds.size();
	}
	else
	{
		if (mPlAttackFinishFlag)
		{
			target = Vector3::ZERO;
		}
		else
		{
			target = commandChara->GetTargetPos();
		}

	}
	// �^�[�Q�b�g���X�V
	mTarget = Vector3::Lerp(mTarget, target, LERP_SPEED);
}

void CameraBattle::EnemyAttack(const BattleCharacter* commandChara, const BattleCharacterManager* bcm)
{
	// �R�}���h�L�����̃��[�V�������擾
	Character::MotionType oldMotion = mCommandCharaMotion;
	mCommandCharaMotion = commandChara->GetMotion();

	// ���[�V�������A�O�񂪍U�����A����U������Ȃ��Ȃ�Attackfinishflag��on
	if (oldMotion == Character::ATTACK && mCommandCharaMotion != Character::ATTACK)
	{
		mEnmAttackFinishFlag = true;
	}

	// ���W���Z�o����
	Vector3 pos = {};
	if (mEnmAttackFinishFlag)
	{
		// �U�����I����Ă�����R�}���h�I���̍��W
		pos = mCommandSelectPos;
	}
	else
	{
		// �v���C���[�̏��
		Vector3 charaPos = commandChara->GetPos();
		float radius = commandChara->GetCapsule().radius;
		float diameter = radius * 2.0f;
		Vector3 front = commandChara->GetFrontXZ();
		Vector3 right = commandChara->GetRightXZ();
		if (charaPos.x < 0) right = -right;

		// ���W���Z�o
		pos.x = charaPos.x + (front.x * radius * ATTACK_DISTANCE) + (right.x * diameter * ATTACK_DISTANCE);
		pos.y = charaPos.y + commandChara->GetHeight() * ATTACK_HEIGHT_RATE;
		pos.z = charaPos.z + (front.z * radius * ATTACK_DISTANCE) + (right.z * diameter * ATTACK_DISTANCE);

	}

	// ���W���X�V
	mPos = Vector3::Lerp(mPos, pos, LERP_SPEED);


	// �^�[�Q�b�g����
	Vector3 target = {  };
	if (commandChara->GetMotion() == Character::ATTACK)
	{
		// �U�����[�V�������Ȃ�A�U������̕��ύ��W���^�[�Q�b�g��

		// ID�擾
		std::vector<int> targetIds = commandChara->GetCommand()->GetTargetObjIDs();

		// ���ς̍��W�����߂�
		for (const auto& id : targetIds)
		{
			BattleCharacter* bc = bcm->GetChara(id);
			target += bc->GetTargetPos();
		}
		target /= targetIds.size();
	}
	else
	{
		if (mEnmAttackFinishFlag)
		{
			target = Vector3::ZERO;
		}
		else
		{
			target = commandChara->GetTargetPos();
		}
	}
	// �^�[�Q�b�g���X�V
	mTarget = Vector3::Lerp(mTarget, target, LERP_SPEED);
}

void CameraBattle::Escape(const BattleCharacter* commandChara)
{
	// �K���ɐݒ�
	mPos = Vector3::Lerp(mPos, mCommandSelectPos, LERP_SPEED);
	mTarget = Vector3::Lerp(mTarget, commandChara->GetTargetPos(), LERP_SPEED);
}

void CameraBattle::ResetAttackFinishFlag()
{
	BattleState::State state = BattleState::GetInstance().GetState();
	BattleState::State old = BattleState::GetInstance().GetOldState();

	if (old == BattleState::State::PLAYER_ATTACK && state != BattleState::State::PLAYER_ATTACK)
	{
		mPlAttackFinishFlag = false;
	}

	if (old == BattleState::State::ENEMY_ATTACK && state != BattleState::State::ENEMY_ATTACK)
	{
		mEnmAttackFinishFlag = false;
	}
}
