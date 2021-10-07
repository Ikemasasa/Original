#include "Character.h"

#include "lib/Primitive3D.h"

#include "GameManager.h"
#include "MeshManager.h"
#include "Singleton.h"

Character::Character(int charaID, Type type)
{
	// メッシュロード
	mMesh = Singleton<MeshManager>::GetInstance().SearchLoad(charaID);
	mCharaID = charaID;
	mType = type;
}

Character::Character(const Character* org)
{
	// モデルだけ一緒に使う
	mMesh = org->mMesh;

	mScale = org->mScale;
	mMass = org->mMass;
	mCharaID = org->mCharaID;
	mType = org->mType;

	mCapsuleParam = org->mCapsuleParam;
	mBoneCollision = org->mBoneCollision;
	//mCol = std::make_unique<Sphere>(mBoneCollision.radius);

	UpdateWorld();
}

void Character::UpdateWorld()
{
	// 各種更新の後に呼び出す
	
	// スキニング
	mMesh->Skinning(GameManager::elapsedTime);

	// 現フレームのワールド行列を計算
	mWorld.SRT(mScale, mAngle, mPos);

	// ボーンの当たり判定更新
	if (GetMotion() == ATTACK && mBoneCollision.radius != 0.0f)
	{
		int frame = GetMotionFrame();
		if (mBoneCollision.beginFrame <= frame) mBoneCollision.enable = true;
		if (mBoneCollision.endFrame < frame)	mBoneCollision.enable = false;
	}
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
	
	// 方向転換
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

	// 方向転換
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

void Character::SetCapsuleParam(float radius)
{
	Vector3 cTop(0.0f, mMesh->mAABB.max.y - radius, 0.0f);
	Vector3 cBottom(0.0f, mMesh->mAABB.min.y + radius, 0.0f);
	mCapsuleParam.centerTop = cTop;
	mCapsuleParam.centerBottom = cBottom;
	mCapsuleParam.radius = radius;
}

AABB Character::GetLocalAABB() const
{
	// スケールだけ適用
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
	// スケール、座標を適用
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

Matrix Character::GetBoneMatrix()
{
	Matrix* mat = mMesh->GetBoneMatrix(mBoneCollision.boneIndex);
	return *mat * mWorld;
}

void Character::GetBoneCollisionParam(Matrix* mat, float* radius)
{
	*mat = GetBoneMatrix();
	*radius = mBoneCollision.radius;
}

void Character::SetBoneCollision(const char* boneName, int beginF, int endF, float radius)
{
	int bIndex = mMesh->FindBone(boneName);
	if (bIndex == -1) return;

	mBoneCollision.boneIndex = bIndex;
	mBoneCollision.beginFrame = beginF;
	mBoneCollision.endFrame = endF;
	mBoneCollision.radius = radius;
	mBoneCollision.enable = true;
}
