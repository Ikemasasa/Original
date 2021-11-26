#include "BossEnemy.h"

#include "CollisionTerrain.h"
#include "Define.h"

BossEnemy::BossEnemy(int charaID) : Enemy(charaID)
{
	// ¿—Êİ’è
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
	// ‰º•ûŒü‚ÌˆÊ’u•â³
	const float RAYPICK_DIST = 0.5f;
	mPos.y = CollisionTerrain::GetHeight(mPos, RAYPICK_DIST);

	UpdateWorld();
}
