#include "CameraTPS.h"

#include "lib/Input.h"
#include "lib/Math.h"

#include "Define.h"
#include "Actor.h"

CameraTPS::CameraTPS() : CameraBase()
{
}

void CameraTPS::Update(const Actor* target)
{
	mDistFromTargetY += Input::GetAxisRY() * 0.1f;
	mAngle.y		 -= Input::GetAxisRX() * DirectX::XMConvertToRadians(1.0f);
	mDistFromTargetY = Math::Clamp(mDistFromTargetY, 1.5f, 7.5f);

	Vector3 p = target->GetPos();

	const float DISTANCE = 15.0f;
	const float ADJUST_TARGET = 2.5f;
	mPos.x = p.x + sinf(mAngle.y) * DISTANCE;
	mPos.y = p.y + mDistFromTargetY;
	mPos.z = p.z + cosf(mAngle.y) * DISTANCE;
	mTarget = Vector3(p.x, p.y + ADJUST_TARGET, p.z);

	UpdateView();
}