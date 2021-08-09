#include "Enemy.h"

#include "lib/Random.h"

#include "CollisionTerrain.h"
#include "Define.h"
#include "GameManager.h"
#include "Player.h"


Enemy::Enemy(int charaID) : Character(charaID, Character::ENEMY)
{
	// シェーダ書き換え
	Shader* shader = new Shader;
	shader->Load(L"Shaders/Character.fx", "VSMain", "PSMain");
	ChangeShader(shader);

	mMass = MASS;
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	mVelocity = Vector3::ZERO;

	// TODO : もっとしっかりしたやつを作ろう
	
	float x = Random::RandomRangef(-40.0f, 40.0f);
	float z = Random::RandomRangef(-40.0f, 40.0f);
	mPos = Vector3(x, 0.0f, z);
	mScale = Vector3(0.02f, 0.02f, 0.02f);

	SetMotion(SkinnedMesh::IDLE);
}

void Enemy::Update(const Vector3& playerPos)
{
	// 存在しないなら return 
	if (!mExist) return;
	
	// ランダムでフィールドを歩き回る
	mVelocity = Vector3::ZERO;
	Move(playerPos);

	// 座標補正
	{
		const float RAYPICK_DIST = 0.25f;
		Vector3 outPos, outNormal;

		// 移動方向(y = posだと、うまいことあたらないから、少し上にあげる)
		Vector3 oldPos = mPos - mVelocity;
		Vector3 sp = Vector3(oldPos.x, oldPos.y + RAYPICK_DIST, oldPos.z); // 移動前の座標
		Vector3 ep = Vector3(mPos.x, mPos.y + RAYPICK_DIST, mPos.z);	   // 移動後の座標
		if (-1 != CollisionTerrain::MoveCheck(sp, ep, &outPos))
		{
			mPos.x = outPos.x;
			mPos.z = outPos.z;
		}

		// 下方向
		sp = Vector3(mPos.x, mPos.y + RAYPICK_DIST, mPos.z);
		ep = Vector3(mPos.x, mPos.y - RAYPICK_DIST, mPos.z);
		float len;
		if (-1 != CollisionTerrain::RayPick(sp, ep, &outPos, &outNormal, &len))
		{
			mPos.y = outPos.y;
		}
	}

	UpdateWorld();
}


void Enemy::Move(const Vector3& playerPos)
{
	// チェイスに移行するかチェック
	if (IsChase(playerPos)) mState = CHASE;

	switch (mState)
	{
	case WAIT: StateWait(); break;
	case WALK: StateWalk(); break;
	case CHASE: StateChase(playerPos); break;
	}

	mPos += mVelocity;
}

void Enemy::StateWait()
{
	SetMotion(SkinnedMesh::IDLE);

	if (mTimer >= WAIT_TO_WALK_SEC)
	{
		// 歩きに移行
		mTimer = 0.0f;
		mMoveAngle = Random::RandomRangef(-Define::PI, Define::PI);
		mState = WALK;
	}
	else
	{
		// タイマー加算
		mTimer += GameManager::elapsedTime;
	}
}

void Enemy::StateWalk()
{
	SetMotion(SkinnedMesh::WALK);

	if (mTimer >= WALK_TO_WAIT_SEC)
	{
		// 待機に移行
		mTimer = 0;
		mState = WAIT;
	}
	else
	{
		// mMoveAngleの方向に移動
		mVelocity.x = sinf(mMoveAngle) * WALK_SPEED;
		mVelocity.z = cosf(mMoveAngle) * WALK_SPEED;

		// 向き補正
		CorrectionAngle();

		// タイマー加算
		mTimer += GameManager::elapsedTime;
	}
}

void Enemy::StateChase(const Vector3& playerPos)
{
	SetMotion(SkinnedMesh::RUN);

	// チェイス終了かチェック
	if (!IsChase(playerPos))
	{
		// 終了なら待機に移行
		mTimer = 0.0f;
		mState = WAIT;
		return;
	}

	Vector3 toPlayer = playerPos - mPos;
	toPlayer.Normalize();

	mVelocity.x = toPlayer.x * RUN_SPEED;
	mVelocity.z = toPlayer.z * RUN_SPEED;
	CorrectionAngle();
}

bool Enemy::IsChase(const Vector3& playerPos)
{
	// Chaseモードに移行するかチェック
	// 条件：距離がTO_CHASE_DIST以内、内積がTO_CHASE_DOT以上
	const float TO_CHASE_DIST = 15.0f;
	const float TO_CHASE_DOT = 0.8f;
	Vector3 toPlayer = playerPos - mPos;
	if (toPlayer.LengthSq() <= TO_CHASE_DIST * TO_CHASE_DIST)
	{
		toPlayer.Normalize();
		float fX = sinf(mAngle.y);
		float fZ = cosf(mAngle.y);
		float dot = fX * toPlayer.x + fZ * toPlayer.z;
		if (dot >= TO_CHASE_DOT)
		{
			return true;
		}
	}

	return false;
}