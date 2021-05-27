#pragma once
#include <memory>

#include "lib/CollisionStructs.h"
#include "lib/GeometricPrimitive.h"
#include "lib/SkinnedMesh.h"
#include "lib/Vector.h"
#include "lib/CollisionStructs.h"

#include "DataBase.h"

class Actor
{
public:
	enum Type
	{
		NONE = -1,
		PLAYER,
		ENEMY,
		TERRAIN,
		
		MAX
	};

private: //protected:

	std::shared_ptr<SkinnedMesh> mMesh			= nullptr;
	std::shared_ptr<GeometricPrimitive> mHit    = nullptr;
	DirectX::XMFLOAT4X4 mWorld					= { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
	Vector3 mPos								= { 0,0,0 };
	Vector3 mScale								= { 1,1,1 };
	Vector3 mAngle								= { 0,0,0 };
	Vector4 mColor								= { 1,1,1,1 };
	int mObjID									= -1; // インスタンスに割り当てられるID(各Managerで0スタートで増えていく)
	int mCharaID								= -1; // キャラクターそのものに割り当てられているID(スタートは種類によって変わる、DataBase参照)
	bool mExist									= true;
	Type mType									= NONE;
	CAPSULE mCapsuleParam;

public:
	Actor(int charaID, Type type);
	Actor(const std::shared_ptr<Actor>& org, Type type);
	virtual ~Actor() = default;


	void UpdateWorld();
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);
	void Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir);

	// ワールド行列更新
	DirectX::XMFLOAT4X4 GetWorldMatrix() { return mWorld; }

	//レイピック
	int RayPickSRT(
		const DirectX::XMFLOAT3& sp,
		const DirectX::XMFLOAT3& ep,
		DirectX::XMFLOAT3* outPos,
		DirectX::XMFLOAT3* outNormal,
		float* outLen);

	// モーション追加
	void AddMotion(const char* filename, SkinnedMesh::MotionType type);

	//ゲッター
	Vector3 GetPos() const		 { return mPos; }
	Vector3 GetScale() const	 { return mScale; }
	Vector3 GetAngle() const	 { return mAngle; }
	Vector4 GetColor() const	 { return mColor; }
	int		GetObjID() const     { return mObjID; }
	int		GetCharaID() const	 { return mCharaID; }
	bool	GetExist() const	 { return mExist; }
	AABB	GetLocalAABB() const { return mMesh->mAABB; }
	AABB    GetAABB() const;
	CAPSULE GetCapsule() const;
	Type	GetType() const		 { return mType; }

	// セッター
	void SetPos(const Vector3& p)	   { mPos = p; }
	void SetScale(const Vector3& s)	   { mScale = s; }
	void SetAngle(const Vector3& a)	   { mAngle = a; }
	void SetAngleY(float y)			   { mAngle.y = y; }
	void SetColor(const Vector4& c)	   { mColor = c; }
	void SetObjID(const int& i)		   { mObjID = i; }
	void SetCharaID(const int& i)	   { mCharaID= i; }
	void SetExist(const bool e)		   { mExist = e; }


	SkinnedMesh::MotionType GetMotion() const { return mMesh->GetMotion(); }
	void SetMotion(SkinnedMesh::MotionType type, bool isLoop = true);

	void ChangeShader(Shader* shader) { mMesh->ChangeShader(shader); }
};