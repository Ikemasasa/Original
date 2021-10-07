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
	mAngle.y		 -= Input::GetAxisRX() * DirectX::XMConvertToRadians(1.25f);
	mDistFromTargetY = Math::Clamp(mDistFromTargetY, 1.5f, 7.5f);

	Vector3 p = target->GetPos();

	const float DISTANCE = 15.0f;
	const float ADJUST_TARGET = 2.5f;
	Vector3 pos;
	pos.x = p.x + sinf(mAngle.y) * DISTANCE;
	pos.y = p.y + mDistFromTargetY;
	pos.z = p.z + cosf(mAngle.y) * DISTANCE;
	mPos = Vector3::Lerp(mPos, pos, LERP_FACTOR);

	Vector3 t;
	t.x = p.x;
	t.y = p.y + ADJUST_TARGET;
	t.z = p.z;
	mTarget = Vector3::Lerp(mTarget, t, LERP_FACTOR);


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

	UpdateView();
}