#include "Character.h"

#include "lib/GeometricPrimitive.h"

#include "GameManager.h"
#include "MeshManager.h"
#include "Singleton.h"

Character::Character(int charaID, Type type)
{
	// メッシュロード
	mMesh = Singleton<MeshManager>::GetInstance().SearchLoad(charaID);
	mCharaID = charaID;
	mType = type;

	// カプセルのサイズ決定
	const Vector3& min = mMesh->mAABB.min;
	const Vector3& max = mMesh->mAABB.max;
	float radius = (max.z - min.z) * 0.5f;
	float x = min.x + ((max.x - min.x) * 0.5f);
	float z = min.z + ((max.z - min.z) * 0.5f);
	Vector3 cTop(x, max.y - radius, z);
	Vector3 cBottom(x, min.y + radius, z);
	mCapsuleParam.centerTop = cTop;
	mCapsuleParam.centerBottom = cBottom;
	mCapsuleParam.radius = radius;
	mHit = std::make_shared<GeometricCapsule>(cTop, cBottom, mCapsuleParam.radius);
}

Character::Character(const Character* org, Type type)
{
	// モデルだけ一緒に使う
	mMesh = org->mMesh;
	mHit = org->mHit;

	mScale = org->mScale;
	mCharaID = org->mCharaID;
	mType = type;

	mCapsuleParam = org->mCapsuleParam;

	UpdateWorld();
}

void Character::UpdateWorld()
{
	// 各種更新の後に呼び出す

	// モーションがあるならskinning
	if (mMesh->GetMotion() != SkinnedMesh::DEFAULT)
	{
		mMesh->Skinning();
	}

	{
		// 現フレームのワールド行列を計算
		DirectX::XMMATRIX S, R, T;
		S = DirectX::XMMatrixScaling(mScale.x, mScale.y, mScale.z);				 // スケール
		R = DirectX::XMMatrixRotationRollPitchYaw(mAngle.x, mAngle.y, mAngle.z); // 回転
		T = DirectX::XMMatrixTranslation(mPos.x, mPos.y, mPos.z);				 // 移動
		DirectX::XMMATRIX world = S * R * T;

		// 現フレームのワールド行列を代入
		DirectX::XMStoreFloat4x4(&mWorld, world);
	}
}

void Character::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	if (mExist)
	{
		DirectX::XMFLOAT4X4 wvp;
		DirectX::XMStoreFloat4x4(&wvp, DirectX::XMLoadFloat4x4(&mWorld) * DirectX::XMLoadFloat4x4(&view) * DirectX::XMLoadFloat4x4(&projection));
		mMesh->Render(wvp, mWorld, lightDir, GameManager::elpsedTime);
		//mHit->Render(wvp, mWorld, lightDir, DirectX::XMFLOAT4(0.8f, 0.2f, 0.2f, 0.2f));
	}
}

void Character::Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	if (mExist)
	{
		DirectX::XMFLOAT4X4 wvp;
		DirectX::XMStoreFloat4x4(&wvp, DirectX::XMLoadFloat4x4(&mWorld) * DirectX::XMLoadFloat4x4(&view) * DirectX::XMLoadFloat4x4(&projection));
		mMesh->Render(shader, wvp, mWorld, lightDir, GameManager::elpsedTime);
	}
}

void Character::CorrectionAngle()
{
	Vector3 direction = mVelocity.GetNormalize();
	
	// 方向転換
	float dX = direction.x;
	float dY = direction.y;
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
		const float ANGLE_ADJUST = 0.2f;
		if (cross < 0) mAngle.y -= ANGLE_ADJUST;
		else mAngle.y += ANGLE_ADJUST;
	}
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
