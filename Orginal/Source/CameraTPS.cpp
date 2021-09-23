#include "CameraTPS.h"

#include "lib/Input.h"
#include "lib/Math.h"

#include "Character.h"
#include "CollisionTerrain.h"
#include "Define.h"

CameraTPS::CameraTPS() : CameraBase()
{
}

void CameraTPS::Update(const Character* target)
{
	mDistFromTargetY += Input::GetAxisRY() * 0.1f;
	mAngle.y		 += Input::GetAxisRX() * DirectX::XMConvertToRadians(1.25f);
	mDistFromTargetY = Math::Clamp(mDistFromTargetY, 1.5f, 7.5f);

	Vector3 p = target->GetPos();

	const float DISTANCE = 15.0f;
	const float ADJUST_TARGET = 2.5f;
	mPos.x = p.x + sinf(mAngle.y) * DISTANCE;
	mPos.y = p.y + mDistFromTargetY;
	mPos.z = p.z + cosf(mAngle.y) * DISTANCE;
	mTarget = Vector3(p.x, p.y + ADJUST_TARGET, p.z);


	Vector3 dist = mPos - mTarget;
	Vector3 hit, normal;
	if (CollisionTerrain::RayPick(mTarget, dist, &hit, &normal) != -1)
	{
		dist = hit - mTarget;
		float lenSq = dist.LengthSq();
		const float MAX_DIST = DISTANCE * Define::ROOT2;
		if (lenSq <= MAX_DIST * MAX_DIST)
		{
			mPos = hit + -dist.GetNormalize();
		}
	}

	UpdateView();
}