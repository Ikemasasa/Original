#include "CameraEvent.h"

void CameraEvent::Update(const Vector3& target)
{
	// モード毎に分岐
	switch (mMode)
	{
	case FIX:
		mTarget = target;
		mPos = mTargetPos; 
		break;

	case LERP:
		// 座標、ターゲットを線形補完で設定
		mPos = Vector3::Lerp(mPos, mTargetPos, mLerpFactor);
		mTarget = Vector3::Lerp(mTarget, target, mLerpFactor);
		break;
	}

	// ビュー更新
	UpdateView();
}
