#include "CameraTitle.h"

#include "Character.h"

void CameraTitle::Update(const Character* chara)
{
	// キャラの後ろ、左のベクトル取得
	Vector3 back = -chara->GetFrontXZ();
	Vector3 left = -chara->GetRightXZ();

	// キャラのサイズ
	float diameter = chara->GetCapsule().radius * 2.0f;

	// 座標ターゲット算出
	Vector3 charaPos = chara->GetPos();
	mPos.x = charaPos.x + (back.x * diameter * 1.5f) + (left.x * diameter * 2.0f);
	mPos.y = POS_Y;
	mPos.z = charaPos.z + (back.z * diameter * 1.5f) + (left.z * diameter * 2.0f);

	AABB aabb = chara->GetLocalAABB();
	mTarget.x = charaPos.x + (left.x * diameter);
	mTarget.y = charaPos.y + aabb.max.y * 0.75f;
	mTarget.z = charaPos.z + (left.z * diameter);

	UpdateView();
}
