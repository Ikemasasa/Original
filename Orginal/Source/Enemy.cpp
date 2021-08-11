#include "Enemy.h"

#include "lib/Random.h"

#include "CollisionTerrain.h"
#include "Define.h"
#include "GameManager.h"
#include "Player.h"


Enemy::Enemy(int charaID) : Character(charaID, Character::ENEMY)
{
	// �V�F�[�_��������
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

	// TODO : �����Ƃ������肵�������낤
	
	float x = Random::RandomRangef(-40.0f, 40.0f);
	float z = Random::RandomRangef(-40.0f, 40.0f);
	mPos = Vector3(x, 0.0f, z);
	mScale = Vector3(0.02f, 0.02f, 0.02f);

	SetMotion(SkinnedMesh::IDLE);
}

void Enemy::Update(const Vector3& playerPos)
{
	// ���݂��Ȃ��Ȃ� return 
	if (!mExist) return;
	
	// �����_���Ńt�B�[���h��������
	mVelocity = Vector3::ZERO;
	Move(playerPos);

	// ���W�␳
	{
		const float RAYPICK_DIST = 0.25f;

		// �ړ�����
		Vector3 outVelocity;
		if (CollisionTerrain::MoveCheck(mPos + Vector3(0, RAYPICK_DIST, 0), mVelocity, GetCapsule().radius, &outVelocity))
		{
			mVelocity = outVelocity;
		}
		mPos += mVelocity;

		// ������
		mPos.y = CollisionTerrain::GetHeight(mPos, RAYPICK_DIST);
	}

	UpdateWorld();
}


void Enemy::Move(const Vector3& playerPos)
{
	// �`�F�C�X�Ɉڍs���邩�`�F�b�N
	if (IsChase(playerPos)) mState = CHASE;

	switch (mState)
	{
	case WAIT: StateWait(); break;
	case WALK: StateWalk(); break;
	case CHASE: StateChase(playerPos); break;
	}
}

void Enemy::StateWait()
{
	SetMotion(SkinnedMesh::IDLE);

	if (mTimer >= WAIT_TO_WALK_SEC)
	{
		// �����Ɉڍs
		mTimer = 0.0f;
		mMoveAngle = Random::RandomRangef(-Define::PI, Define::PI);
		mState = WALK;
	}
	else
	{
		// �^�C�}�[���Z
		mTimer += GameManager::elapsedTime;
	}
}

void Enemy::StateWalk()
{
	SetMotion(SkinnedMesh::WALK);

	if (mTimer >= WALK_TO_WAIT_SEC)
	{
		// �ҋ@�Ɉڍs
		mTimer = 0;
		mState = WAIT;
	}
	else
	{
		// mMoveAngle�̕����Ɉړ�
		mVelocity.x = sinf(mMoveAngle) * WALK_SPEED;
		mVelocity.z = cosf(mMoveAngle) * WALK_SPEED;

		// �����␳
		CorrectionAngle();

		// �^�C�}�[���Z
		mTimer += GameManager::elapsedTime;
	}
}

void Enemy::StateChase(const Vector3& playerPos)
{
	SetMotion(SkinnedMesh::RUN);

	// �`�F�C�X�I�����`�F�b�N
	if (!IsChase(playerPos))
	{
		// �I���Ȃ�ҋ@�Ɉڍs
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
	// Chase���[�h�Ɉڍs���邩�`�F�b�N
	// �����F������TO_CHASE_DIST�ȓ��A���ς�TO_CHASE_DOT�ȏ�
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