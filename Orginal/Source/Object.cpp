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

int Object::RayPickSRT(const DirectX::XMFLOAT3& sp, const DirectX::XMFLOAT3& ep, DirectX::XMFLOAT3* outPos, DirectX::XMFLOAT3* outNormal, float* outLen)
{
	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&mWorld);
	DirectX::XMMATRIX worldInverse = DirectX::XMMatrixInverse(nullptr, world);

	// 前回のオブジェクト空間でのレイに変換
	DirectX::XMVECTOR vSp = DirectX::XMLoadFloat3(&sp);
	DirectX::XMVECTOR vEp = DirectX::XMLoadFloat3(&ep);
	vSp = DirectX::XMVector3TransformCoord(vSp, worldInverse);
	vEp = DirectX::XMVector3TransformCoord(vEp, worldInverse);

	// レイピック
	DirectX::XMFLOAT3 fSp, fEp;
	DirectX::XMStoreFloat3(&fSp, vSp);
	DirectX::XMStoreFloat3(&fEp, vEp);

	float len = 0.0f;
	int ret = -1;
	ret = mMesh->RayPick(fSp, fEp, outPos, outNormal, &len);

	if (ret != -1)
	{
		// オブジェクト空間から現在のワールド空間へ変換
		DirectX::XMVECTOR op = DirectX::XMLoadFloat3(outPos);
		DirectX::XMVECTOR on = DirectX::XMLoadFloat3(outNormal);
		op = DirectX::XMVector3TransformCoord(op, world);
		on = DirectX::XMVector3TransformCoord(on, world);

		DirectX::XMStoreFloat3(outPos, op);
		DirectX::XMStoreFloat3(outNormal, on);
	}

	return ret;
}
