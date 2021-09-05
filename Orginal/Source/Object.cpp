#include "Object.h"

#include "GameManager.h"
#include "MeshManager.h"
#include "Singleton.h"


Object::Object(int id)
{
	mMesh = Singleton<MeshManager>().GetInstance().SearchLoad(id);
	mID = id;
}

Object::Object(const Object* obj)
{
	mMesh = obj->mMesh;
	mID = obj->mID;

	mScale = obj->mScale;
}

void Object::UpdateWorld()
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

void Object::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	DirectX::XMFLOAT4X4 wvp;
	DirectX::XMStoreFloat4x4(&wvp, DirectX::XMLoadFloat4x4(&mWorld) * DirectX::XMLoadFloat4x4(&view) * DirectX::XMLoadFloat4x4(&projection));
	mMesh->Render(wvp, mWorld, lightDir, GameManager::elapsedTime);
}

void Object::Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	DirectX::XMFLOAT4X4 wvp;
	DirectX::XMStoreFloat4x4(&wvp, DirectX::XMLoadFloat4x4(&mWorld) * DirectX::XMLoadFloat4x4(&view) * DirectX::XMLoadFloat4x4(&projection));
	mMesh->Render(shader, wvp, mWorld, lightDir, GameManager::elapsedTime);
}

int Object::RayPickSRT(const Vector3& sp, const Vector3& velocity, Vector3* outPos, Vector3* outNormal)
{
	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&mWorld);
	DirectX::XMMATRIX worldInverse = DirectX::XMMatrixInverse(nullptr, world);

	DirectX::XMVECTOR spInv  = DirectX::XMVector3TransformCoord(sp, worldInverse);
	DirectX::XMVECTOR velInv = DirectX::XMVector3TransformCoord(velocity, worldInverse);

	int ret = mMesh->RayPick(spInv, velInv, outPos, outNormal);
	if (ret != -1)
	{
		*outPos    = DirectX::XMVector3TransformCoord(*outPos,    world);
		*outNormal = DirectX::XMVector3TransformCoord(*outNormal, world);
	}

	return ret;
}
