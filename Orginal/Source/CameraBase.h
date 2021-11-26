#pragma once
#include <memory>

#include "lib/Matrix.h"
#include "lib/Vector.h"

// �O���錾
class Character;
class BattleCharacter;
class BattleCharacterManager;


// �J�����̊��N���X(�K�v�����Ȃ��̂��l�ߍ���)
class CameraBase
{
protected: // �ϐ�

	// �r���[�֌W
	Vector3 mPos		= {};	// ���W
	Vector3 mTarget		= {};	// �����_
	float mZenithAngle  = 0.0f; // �V���p:rad(�c�̊p�x)
	float mAzimuthAngle = 0.0f; // ���ʊp:rad(���̊p�x)
	Matrix mView	    = {};   // �r���[�s��

	// �v���W�F�N�V�����֌W
	float mFov = 0.0f;		 // ����p
	float mAspect = 0.0f;	 // �A�X�y�N�g��
	float mNear = 0.0f;		 // �j�A�N���b�v����
	float mFar = 0.0f;		 // �t�@�[�N���b�v����
	Matrix mProjection = {}; // �v���W�F�N�V�����s��

	// �J�����̕����̃x�N�g��
	Vector3 mForward = {};
	Vector3 mRight = {};

	// �h�炵�֌W
	bool mIsShake = false;		 // �h�炵�����̃t���O
	float mShakeDuration = 0.0f; // �h�炵�̎���
	float mShakeStrength = 0.0f; // �h�炵�̋���
	float mShakeElapsed = 0.0f;  // �h�炵�̌o�ߎ���


protected: // �֐�

	// �r���[�s��X�V
	void UpdateView();

	// �h�炵�X�V
	void UpdateShake(float elapsedTime);

	// �p�x������W���Z�o
	Vector3 CalcPosFromAngle(float distance);

public:
	// �R���X�g���N�^
	CameraBase(); 

	// �f�X�g���N�^
	virtual ~CameraBase() = default;

	// �������֐�
	virtual void Initialize(const Vector3& target, float distance = 5.0f);

	// �X�V�֐�
	virtual void Update(const Vector3& target) {};
	virtual void Update(const Character* target) {};
	virtual void Update(const BattleCharacter* commandChara, const BattleCharacterManager* bcm) {};

	// �h�炵�̃p�����[�^�ݒ�֐�
	void Shake(float duration, float strength);

	//	���s���e�s��ݒ�֐�
	void SetOrthoMatrix(float w, float h, float znear, float zfar);					

	//	�������e�s��ݒ�֐�
	void SetPerspectiveMatrix(float fov, float aspect, float znear, float zfar);	

	// �Q�b�^�[
	Matrix GetViewMatrix() const { return mView; }				// �r���[�s��擾�֐�
	Matrix GetProjectionMatrix() const { return mProjection; }	// ���e�s��擾�֐�
	Vector3 GetPos() const { return mPos; }						// ���W�擾
	float GetZenithAngle() const { return mZenithAngle; }		// �V���p�擾
	float GetAzimuthAngle() const { return mAzimuthAngle; }		// ���ʊp�擾
	Vector3 GetForward() const { return mForward; }				// �O���x�N�g���擾
	Vector3 GetForwardXZ() const { return Vector3(mForward.x, 0.0f, mForward.z).GetNormalize(); } // XZ���ʂ̑O���x�N�g���擾
	Vector3 GetRight() const { return mRight; }					// �E���x�N�g���擾
	Vector3 GetRightXZ() const { return Vector3(mRight.x, 0.0f, mRight.z).GetNormalize(); } // XZ���ʂ̉E���x�N�g���擾
};