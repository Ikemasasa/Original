#pragma once
#include "CameraBase.h"

class CameraEvent : public CameraBase
{
public: // �萔
	// �J�����̃��[�hEnum
	enum Mode
	{
		NONE,
		FIX,  // �Œ�J����
		LERP, // ���`�⊮�J����
	};

private: // �ϐ�
	Mode mMode = NONE; // ���[�h�ۑ�
	Vector3 mTargetPos; // �ړ���̍��W
	float mLerpFactor;  // ���`�⊮�̌W��

public: // �֐�
	void Update(const Vector3& target) override;

	void SetMode(Mode mode) { mMode = mode; }					// ���[�h��ݒ�
	void SetTargetPos(const Vector3& tp) { mTargetPos = mPos; } // �ړ���̍��W��ݒ�
	void SetLerpFactor(float factor) { mLerpFactor = factor; }  // ���`�⊮�̌W����ݒ�
	void SetPos(const Vector3& pos) { mPos = pos; }
	void SetTarget(const Vector3& target) { mTarget = target; }
};