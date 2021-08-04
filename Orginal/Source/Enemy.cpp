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
		Vector3 outPos, outNormal;

		// �ړ�����(y = pos���ƁA���܂����Ƃ�����Ȃ�����A������ɂ�����)
		Vector3 oldPos = mPos - mVelocity;
		Vector3 sp = Vector3(oldPos.x, oldPos.y + RAYPICK_DIST, oldPos.z); // �ړ��O�̍��W
		Vector3 ep = Vector3(mPos.x, mPos.y + RAYPICK_DIST, mPos.z);	   // �ړ���̍��W
		if (-1 != CollisionTerrain::MoveCheck(sp, ep, &outPos))
		{
			mPos.x = outPos.x;
			mPos.z = outPos.z;
		}

		// ������
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
	// �`�F�C�X�Ɉڍs���邩�`�F�b�N
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