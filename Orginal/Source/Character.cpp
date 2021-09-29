#include "Character.h"

#include "lib/Primitive3D.h"

#include "GameManager.h"
#include "MeshManager.h"
#include "Singleton.h"

Character::Character(int charaID, Type type)
{
	// ���b�V�����[�h
	mMesh = Singleton<MeshManager>::GetInstance().SearchLoad(charaID);
	mCharaID = charaID;
	mType = type;

	// �J�v�Z���̃T�C�Y����
	const Vector3& min = mMesh->mAABB.min;
	const Vector3& max = mMesh->mAABB.max;
	float radius = (max.z - min.z);// ���a�ɂ��Ă�(�������̕����ۂ�����)
	float x = min.x + ((max.x - min.x) * 0.5f);
	float z = min.z + ((max.z - min.z) * 0.5f);
	Vector3 cTop(x, max.y - radius, z);
	Vector3 cBottom(x, min.y + radius, z);
	mCapsuleParam.centerTop = cTop;
	mCapsuleParam.centerBottom = cBottom;
	mCapsuleParam.radius = radius;
	mHit = std::make_shared<Capsule>(cTop, cBottom, mCapsuleParam.radius);
}

Character::Character(const Character* org)
{
	// ���f�������ꏏ�Ɏg��
	mMesh = org->mMesh;
	mHit = org->mHit;

	mScale = org->mScale;
	mMass = org->mMass;
	mCharaID = org->mCharaID;
	mType = org->mType;

	mCapsuleParam = org->mCapsuleParam;

	UpdateWorld();
}

void Character::UpdateWorld()
{
	// �e��X�V�̌�ɌĂяo��
	
	// �X�L�j���O
	mMesh->Skinning(GameManager::elapsedTime);

	// ���t���[���̃��[���h�s����v�Z
	mWorld.SRT(mScale, mAngle, mPos);
}

void Character::Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir)
{
	if (mExist)
	{
		Matrix wvp = mWorld * view * proj;
		mMesh->Render(shader, wvp, mWorld, lightDir);
	}
}

void Character::CorrectionAngle()
{
	Vector3 direction = mVelocity.GetNormalize();
	
	// �����]��
	float dX = direction.x;
	float dY = direction.z;
	float aX = sinf(mAngle.y);
	float aY = cosf(mAngle.y);

	float dot   = dX * aX + dY * aY;
	float cross = dX * aY - dY * aX;

	const float FORCE_ROTATE_VALUE = 0.96f;
	if (dot >= FORCE_ROTATE_VALUE)
	{
		float angle = atan2f(dX, dY);
		mAngle.y = angle;
	}
	else
	{
		const float ANGLE_ADJUST = 0.3f;
		if (cross < 0) mAngle.y -= ANGLE_ADJUST;
		else mAngle.y += ANGLE_ADJUST;
	}
}

void Character::CorrectionAngle(const Vector3& dirN)
{
	Vector3 direction = dirN;

	// �����]��
	float dX = direction.x;
	float dY = direction.z;
	float aX = sinf(mAngle.y);
	float aY = cosf(mAngle.y);

	float dot = dX * aX + dY * aY;
	float cross = dX * aY - dY * aX;

	const float FORCE_ROTATE_VALUE = 0.96f;
	if (dot >= FORCE_ROTATE_VALUE)
	{
		float angle = atan2f(dX, dY);
		mAngle.y = angle;
	}
	else
	{
		const float ANGLE_ADJUST = 0.2f;
		if (cross < 0) mAngle.y -= ANGLE_ADJUST;
		else mAngle.y += ANGLE_ADJUST;
	}
}

AABB Character::GetLocalAABB() const
{
	// �X�P�[�������K�p
	Vector3 scale = GetScale();
	AABB aabb = mMesh->mAABB;
	aabb.min.x = aabb.min.x * scale.x;
	aabb.min.y = aabb.min.y * scale.y;
	aabb.min.z = aabb.min.z * scale.z;
	aabb.max.x = aabb.max.x * scale.x;
	aabb.max.y = aabb.max.y * scale.y;
	aabb.max.z = aabb.max.z * scale.z;

	return aabb;
}

AABB Character::GetAABB() const
{
	// �X�P�[���A���W��K�p
	Vector3 pos = GetPos();
	Vector3 scale = GetScale();
	AABB aabb = mMesh->mAABB;
	aabb.min.x = aabb.min.x * scale.x + pos.x;
	aabb.min.y = aabb.min.y * scale.y + pos.y;
	aabb.min.z = aabb.min.z * scale.z + pos.z;
	aabb.max.x = aabb.max.x * scale.x + pos.x;
	aabb.max.y = aabb.max.y * scale.y + pos.y;
	aabb.max.z = aabb.max.z * scale.z + pos.z;
	return aabb;
}

CAPSULE Character::GetCapsule() const
{
	Vector3 pos = GetPos();
	Vector3 scale = GetScale();
	CAPSULE c = mCapsuleParam;
	c.centerTop.x = c.centerTop.x * scale.x + pos.x;
	c.centerTop.y = c.centerTop.y * scale.y + pos.y;
	c.centerTop.z = c.centerTop.z * scale.z + pos.z;
	c.centerBottom.x = c.centerBottom.x * scale.x + pos.x;
	c.centerBottom.y = c.centerBottom.y * scale.y + pos.y;
	c.centerBottom.z = c.centerBottom.z * scale.z + pos.z;
	c.radius = c.radius * scale.z;
	return c;
}
