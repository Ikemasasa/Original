#pragma once
#include <memory>

#include "lib/CollisionStructs.h"
#include "lib/Matrix.h"
#include "lib/SkinnedMesh.h"
#include "lib/Vector.h"

class GeometricPrimitive;

class Character
{
public:
	enum Type
	{
		NONE = -1,
		PLAYER,
		ENEMY,
		NPC,

		MAX
	};

private:
	std::shared_ptr<SkinnedMesh> mMesh = nullptr;
	std::shared_ptr<GeometricPrimitive> mHit = nullptr;
	Matrix mWorld = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
	int mObjID = -1; // �C���X�^���X�Ɋ��蓖�Ă���ID(�eManager��0�X�^�[�g�ő����Ă���)
	int mCharaID = -1; // �L�����N�^�[���̂��̂Ɋ��蓖�Ă��Ă���ID(�X�^�[�g�͎�ނɂ���ĕς��ADataBase�Q��)
	CAPSULE mCapsuleParam;

protected:
	Vector3 mPos = { 0,0,0 };
	Vector3 mVelocity = { 0,0,0 };
	Vector3 mScale = { 1, 1, 1 };
	Vector3 mAngle = { 0,0,0 };
	float mMass = 0;		//����
	bool mExist = true;
	Type mType = NONE;

public:
	Character(int charaID, Type type);
	Character(const Character* org);
	virtual ~Character() = default;

	void UpdateWorld();
	void Render(const Matrix& view, const Matrix& proj, const Vector4& lightDir);
	void Render(const Shader * shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);

	void CorrectionAngle();	// mVelocity�����Ɍ���
	void CorrectionAngle(const Vector3& dirN); // dirN�����Ɍ���

	// ���[�V�����ǉ�
	void AddMotion(const char* filename, SkinnedMesh::MotionType type) { mMesh->AddMotion(filename, type); }

	//�Q�b�^�[
	Vector3 GetPos() const { return mPos; }
	Vector3 GetVelocity() const { return mVelocity; }
	Vector3 GetScale() const { return mScale; }
	Vector3 GetAngle() const { return mAngle; }
	int		GetObjID() const { return mObjID; }
	int		GetCharaID() const { return mCharaID; }
	bool	GetExist() const { return mExist; }
	AABB	GetLocalAABB() const;
	AABB    GetAABB() const;
	CAPSULE GetCapsule() const;
	Type	GetType() const { return mType; }
	float	GetMass() const { return mMass; }

	// �Z�b�^�[
	void SetPos(const Vector3& p) { mPos = p; }
	void SetVelocity(const Vector3 & v) { mVelocity = v; }
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