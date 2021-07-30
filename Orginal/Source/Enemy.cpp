#include "Enemy.h"

#include "lib/Random.h"

#include "CollisionTerrain.h"
#include "Define.h"
#include "Player.h"


Enemy::Enemy(int charaID) : Actor(charaID, Actor::ENEMY)
{
	mVelocity = Vector3::ZERO;

	Shader* shader = new Shader;
	shader->Load(L"Shaders/Character.fx", "VSMain", "PSMain");
	ChangeShader(shader);
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	mVelocity = Vector3::ZERO;

	// TODO : もっとしっかりしたやつを作ろう
	float x = rand() % 800 * 0.1f;
	float z = rand() % 800 * 0.1f;
	SetPos(Vector3(x, 0.0f, z));
	SetScale(Vector3(0.02f, 0.02f, 0.02f));
	SetMotion(SkinnedMesh::IDLE);
}

void Enemy::Update(const Vector3& playerPos)
{
	if (!GetExist()) return;

	// ランダムでフィールドを歩き回る
	Vector3 pos = GetPos();
	Vector3 sp = { pos.x, pos.y + 0.5f, pos.z };
	Vector3 ep = { pos.x, pos.y - 0.5f, pos.z };
	Vector3 outPos, outNor;
	float len = 0.0f;
	if (-1 != CollisionTerrain::RayPick(sp, ep, &outPos, &outNor, &len))
	{
		pos.y = outPos.y;
	}

	mVelocity = Vector3::ZERO;
	DecideMoveState(playerPos);

	SetPos(GetPos() + mVelocity);

	UpdateWorld();
}

void Enemy::DecideMoveState(const Vector3& playerPos)
{
	const int WAIT_TO_WALK_SEC = 5;
	const int WALK_TO_WAIT_SEC = 4;

	constexpr float WALK_SPEED = 0.08f;
	constexpr float RUN_SPEED = 0.16f;

	constexpr float CHASE_DIST = 12.5f;
	constexpr float CHASE_ANGLE = DirectX::XMConvertToRadians(35.0f);

	Vector3 dir = {};
	float distSqFromPlayer = 0.0f;
	Vector3 front = {};
	auto CheckChase = [&]
	{
		// 距離
		dir = playerPos - GetPos();
		distSqFromPlayer = dir.LengthSq();
		dir.Normalize();
	
		front.x = sinf(GetAngle().y);
		front.y = 0.0f;
		front.z = cosf(GetAngle().y);
		front.Normalize();

		// 角度
		float angle = acosf(front.Dot(dir));

		// 距離がCHASE_DIST以下で、角度がCHASE_ANGLE以下なら
		if (distSqFromPlayer <= CHASE_DIST * CHASE_DIST &&
			angle <= CHASE_ANGLE)
		{
			mState = CHASE;
			mTimer = 0;
		}
	};

	CheckChase();

	switch (mState)
	{
	case WAIT: // ぼったち
		SetMotion(SkinnedMesh::IDLE);
		if (mTimer >= WAIT_TO_WALK_SEC * Define::FRAMERATE)
		{
			mState = WALK;
			float angle = Random::RandomRangef(0.0f, 360.0f);
			SetAngleY(DirectX::XMConvertToRadians(angle));
			mTimer = 0;
		}
		else
		{
			++mTimer;
		}

		break;

	case WALK: // 前進
		SetMotion(SkinnedMesh::WALK);
		if (mTimer >= WALK_TO_WAIT_SEC * Define::FRAMERATE)
		{
			mState = WAIT;
			mTimer = 0;
		}
		else
		{
			mVelocity = front * WALK_SPEED;

			++mTimer;
		}

		break;

	case CHASE:// プレイヤーを追いかける
		SetMotion(SkinnedMesh::RUN);

		Vector3 vec0 = { sinf(0.0f), 0.0f, cosf(0.0f) };
		float angle = acosf(vec0.Dot(dir));
		Vector3 cross = vec0.Cross(dir);

		if (cross.y < 0) angle = DirectX::XMConvertToRadians(360.0f) - angle;
		SetAngleY(angle);

		mVelocity = dir * RUN_SPEED;

		// 距離がCHASE_DIST以上なら
		if (distSqFromPlayer >= CHASE_DIST * CHASE_DIST)
		{
			mState = WAIT;
		}
		break;

	}
}
