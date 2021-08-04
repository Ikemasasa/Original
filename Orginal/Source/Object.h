#pragma once

#include <DirectXMath.h>
#include <memory>

#include "lib/Vector.h"

class SkinnedMesh;
class Shader;

class Object
{
private:
	std::shared_ptr<SkinnedMesh> mMesh = nullptr;
	DirectX::XMFLOAT4X4 mWorld = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
	int mID = -1;

protected:
	Vector3 mPos = { 0,0,0 };
	Vector3 mScale = { 1, 1, 1 };
	Vector3 mAngle = { 0,0,0 };

public:
	Object(int id);
	Object(const Object* obj);
	~Object() = default;

	void UpdateWorld();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);

	//レイピック
	int RayPickSRT(
		const DirectX::XMFLOAT3& sp,
		const DirectX::XMFLOAT3& ep,
		DirectX::XMFLOAT3* outPos,
		DirectX::XMFLOAT3* outNormal,
		float* outLen);
	

	//ゲッター
	Vector3 GetPos() const { return mPos; }
	Vector3 GetScale() const { return mScale; }
	Vector3 GetAngle() const { return mAngle; }
	int GetID() const { return mID; }

	// セッター
	void SetPos(const Vector3& p) { mPos = p; }
	void SetScale(const Vector3& s) { mScale = s; }
	void SetAngle(const Vector3& a) { mAngle = a; }
	void SetAngleY(float y) { mAngle.y = y; }
	void SetID(const int id) { mID = id; }

};