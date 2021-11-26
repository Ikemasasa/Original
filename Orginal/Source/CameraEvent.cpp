#include "CameraEvent.h"

void CameraEvent::Update(const Vector3& target)
{
	// ���[�h���ɕ���
	switch (mMode)
	{
	case FIX:
		mTarget = target;
		mPos = mTargetPos; 
		break;

	case LERP:
		// ���W�A�^�[�Q�b�g����`�⊮�Őݒ�
		mPos = Vector3::Lerp(mPos, mTargetPos, mLerpFactor);
		mTarget = Vector3::Lerp(mTarget, target, mLerpFactor);
		break;
	}

	// �r���[�X�V
	UpdateView();
}
