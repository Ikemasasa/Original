#include "CameraTitle.h"

#include "Character.h"

void CameraTitle::Update(const Character* chara)
{
	// キャラの前、右のベクトル取得
	Matrix world = chara->GetWorld();
	Vector3 back = Vector3(world._13, 0.0f, world._33).GetNormalize();
	Vector3 left = -Vector3(world._11, 0.0f, world._31).GetNormalize();

	// キャラのサイズ
	float diameter = chara->GetCapsule().radius * 2.0f;

	// 座標ターゲット算出
	Vector3 charaPos = chara->GetPos();
	mPos.x = charaPos.x + (back.x * diameter * 1.5f) + (left.x * diameter * 2.0f);
	mPos.y = POS_Y;
	mPos.z = charaPos.z + (back.z * diameter * 1.5f) + (left.z * diameter * 2.0f);;

	AABB aabb = chara->GetLocalAABB();
	mTarget.x = charaPos.x + (left.x * diameter);
	mTarget.y = charaPos.y + aabb.max.y * 0.75f;
	mTarget.z = charaPos.z + (left.z * diameter);

	UpdateView();
}
