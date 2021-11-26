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
	// 入力情報を取得
	float inputX = Input::GetAxisRX();
	float inputY = Input::GetAxisRY();

	// 入力情報をradianに変換して角度を算出
	mAzimuthAngle += Math::ToRadian(inputX * SPEED_X);
	mZenithAngle  -= Math::ToRadian(inputY * SPEED_Y);

	// 縦方向の角度制限
	float min = Math::ToRadian(50.0f);
	float max = Math::ToRadian(120.0f);
	mZenithAngle = Math::Clamp(mZenithAngle, min, max);

	// ターゲット設定
	mTarget = Vector3::Lerp(mTarget, target, LERP_FACTOR);

	// 座標を算出、設定
	Vector3 pos = CalcPosFromAngle(DISTANCE);
	mPos = Vector3::Lerp(mPos, pos, LERP_FACTOR);

	// 地形にRayを飛ばして地形の裏側に行かないようにする
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

	// ビュー更新
	UpdateView();
}