#include "BossEnemy.h"

#include "CollisionTerrain.h"
#include "Define.h"

BossEnemy::BossEnemy(int charaID) : Enemy(charaID)
{
	// 質量設定
	mMass = MASS;
}

BossEnemy::~BossEnemy()
{
}

void BossEnemy::Initialize()
{
	Enemy::Initialize();
	mMass = MASS;
}

void BossEnemy::Update(const Vector3& playerPos)
{
	// 下方向の位置補正
	const float RAYPICK_DIST = 0.5f;
	mPos.y = CollisionTerrain::GetHeight(mPos, RAYPICK_DIST);

	UpdateWorld();
}
