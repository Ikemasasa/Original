#include "CameraTPS.h"

#include "lib/Input.h"
#include "lib/Math.h"

#include "Character.h"
#include "CollisionTerrain.h"
#include "Define.h"

CameraTPS::CameraTPS() : CameraBase()
{
	mAzimuthAngle = 0.0f;
	mZenithAngle = Math::ToRadian(90.0f);
}

void CameraTPS::Update(const Vector3& target)
{
	// ���͏����擾
	float inputX = Input::GetAxisRX();
	float inputY = Input::GetAxisRY();

	// ���͏���radian�ɕϊ����Ċp�x���Z�o
	mAzimuthAngle += Math::ToRadian(inputX * SPEED_X);
	mZenithAngle  -= Math::ToRadian(inputY * SPEED_Y);

	// �c�����̊p�x����
	float min = Math::ToRadian(50.0f);
	float max = Math::ToRadian(120.0f);
	mZenithAngle = Math::Clamp(mZenithAngle, min, max);

	// �^�[�Q�b�g�ݒ�
	mTarget = Vector3::Lerp(mTarget, target, LERP_FACTOR);

	// ���W���Z�o�A�ݒ�
	Vector3 pos = CalcPosFromAngle(DISTANCE);
	mPos = Vector3::Lerp(mPos, pos, LERP_FACTOR);

	// �n�`��Ray���΂��Ēn�`�̗����ɍs���Ȃ��悤�ɂ���
	Vector3 dist = mPos - mTarget;
	Vector3 hit, normal;
	if (CollisionTerrain::RayPickOrg(mTarget, dist, &hit, &normal) != -1)
	{
		dist = hit - mTarget;
		float lenSq = dist.LengthSq();
		const float MAX_DIST = DISTANCE * Define::ROOT2;
		if (lenSq <= MAX_DIST * MAX_DIST)
		{
			mPos = hit + -dist.GetNormalize();
		}
	}

	// �r���[�X�V
	UpdateView();
}