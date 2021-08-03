#pragma once
#include <memory>

#include "lib/CollisionStructs.h"
#include "lib/Vector.h"

#include "lib/SkinnedMesh.h"

class GeometricPrimitive;

class Character
{
public:
	enum Type
	{
		NONE = -1,
		PLAYER,
		ENEMY,

		MAX
	};

private:
	std::shared_ptr<SkinnedMesh> mMesh = nullptr;
	std::shared_ptr<GeometricPrimitive> mHit = nullptr;
	DirectX::XMFLOAT4X4 mWorld = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
	int mObjID = -1; // インスタンスに割り当てられるID(各Managerで0スタートで増えていく)
	int mCharaID = -1; // キャラクターそのものに割り当てられているID(スタートは種類によって変わる、DataBase参照)
	CAPSULE mCapsuleParam;

protected:
	Vector3 mPos = { 0,0,0 };
	Vector3 mVelocity = { 0,0,0 };
	Vector3 mScale = { 1, 1, 1 };
	Vector3 mAngle = { 0,0,0 };
	int mMass = 0;		//質量
	bool mExist = true;
	Type mType = NONE;

public:
	Character(int charaID, Type type);
	Character(const Character* org, Type type);
	virtual ~Character() = default;


	void UpdateWorld();
	void Render(const DirectX::XMFLOAT4X4 & view, const DirectX::XMFLOAT4X4 & projection, const DirectX::XMFLOAT4 & lightDir);
	void Render(const Shader * shader, const DirectX::XMFLOAT4X4 & view, const DirectX::XMFLOAT4X4 & projection, const DirectX::XMFLOAT4 & lightDir);


	void CorrectionAngle();	// mVelocity方向に向く
	void CorrectionAngle(const Vector3& dirN); // distN方向に向く

	// モーション追加
	void AddMotion(const char* filename, SkinnedMesh::MotionType type) { mMesh->AddMotion(filename, type); }

	//ゲッター
	Vector3 GetPos() const { return mPos; }
	Vector3 GetScale() const { return mScale; }
	Vector3 GetAngle() const { return mAngle; }
	int		GetObjID() const { return mObjID; }
	int		GetCharaID() const { return mCharaID; }
	bool	GetExist() const { return mExist; }
	AABB	GetLocalAABB() const;
	AABB    GetAABB() const;
	CAPSULE GetCapsule() const;
	Type	GetType() const { return mType; }

	// セッター
	void SetPos(const Vector3 & p) { mPos = p; }
	void SetScale(const Vector3 & s) { mScale = s; }
	void SetAngle(const Vector3 & a) { mAngle = a; }
	void SetAngleY(float y) { mAngle.y = y; }
	void SetObjID(const int& i) { mObjID = i; }
	void SetCharaID(const int& i) { mCharaID = i; }
	void SetExist(const bool e) { mExist = e; }


	SkinnedMesh::MotionType GetMotion() const { return mMesh->GetMotion(); }
	void SetMotion(SkinnedMesh::MotionType type, bool isLoop = true) { mMesh->SetMotion(type, isLoop); }

	void ChangeShader(Shader * shader) { mMesh->ChangeShader(shader); }
	bool IsMotionFinished() const { return mMesh->IsMotionFinished(); }
};