#include "BossEnemy.h"

#include "CollisionTerrain.h"
#include "Define.h"

BossEnemy::BossEnemy(int charaID) : Enemy(charaID)
{

}

BossEnemy::~BossEnemy()
{
}

void BossEnemy::Initialize()
{
	SetScale(Vector3(0.025f, 0.025f, 0.025f));
	SetPos(Vector3(-110.0f, 10.0f, 0.0f)); // TODO : ボスエネミー用の座標データ作る
	SetAngleY(Define::PI / 2.0f);
	SetMotion(SkinnedMesh::IDLE);
}

void BossEnemy::Update(const Vector3& playerPos)
{
	// 下方向の位置補正
	const float RAYPICK_DIST = 0.5f;
	mPos.y = CollisionTerrain::GetHeight(mPos, RAYPICK_DIST);

	UpdateWorld();
}
