#pragma once
#include <memory>

#include "lib/CollisionStructs.h"
#include "lib/Matrix.h"
#include "lib/SkinnedMesh.h"
#include "lib/Vector.h"
#include "lib/Primitive3D.h"


class Character
{
	static constexpr float DEFAULT_MOTION_BLEND_SPEED = 0.15f;

public:
	enum Type
	{
		NONE = -1,
		PLAYER,
		ENEMY,
		NPC,

		MAX
	};

	enum MotionType
	{
		DEFAULT,
		IDLE,
		WALK,
		RUN,
		ATTACK,
		GUARD,
		DIE,
		DAMAGE,
		USE_ITEM,
		UNIQUE1,
		UNIQUE2,
		UNIQUE3,

		MOTION_MAX,
	};

	struct BoneCollision
	{
		int boneIndex = -1;
		int beginFrame = 0;
		int endFrame = 0;
		float radius = 0.0f;
		bool enable = false;
	};

private:
	std::shared_ptr<SkinnedMesh> mMesh = nullptr;
	std::shared_ptr<Primitive3D> mHit = nullptr;
	std::unique_ptr<Primitive3D> mCol = nullptr;
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

	BoneCollision mBoneCollision;

public:
	Character(int charaID, Type type);
	Character(const Character* org);
	virtual ~Character() = default;

	void UpdateWorld();
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);

	void CorrectionAngle();	// mVelocity�����Ɍ���
	void CorrectionAngle(const Vector3& dirN); // dirN�����Ɍ���
	void SetCapsuleParam(float radius);

	// ���[�V�����ǉ�
	void AddMotion(const char* filename, MotionType type) { mMesh->AddMotion(filename, type); }


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

	// ���[�V�����֘A
	MotionType GetMotion() const { return (MotionType)mMesh->GetMotion(); }
	void SetMotion(MotionType type, float blendSpeed = DEFAULT_MOTION_BLEND_SPEED) { mMesh->SetMotion(type, blendSpeed); }
	void SetMotionOnce(MotionType type, MotionType nextType, float blendSpeed = DEFAULT_MOTION_BLEND_SPEED) { mMesh->SetMotionOnce(type, nextType, blendSpeed); }
	void SetMotionStopLastFrame(MotionType type, float blendSpeed = DEFAULT_MOTION_BLEND_SPEED) { mMesh->SetMotionStopLastFrame(type, blendSpeed); }
	bool IsMotionFinished() const { return mMesh->IsMotionFinished(); }

	int GetMotionFrame() const { return mMesh->GetFrame(); }
	Matrix GetBoneMatrix(); 
	void GetBoneCollisionParam(Matrix* mat, float* radius);
	void SetBoneCollision(const char* boneName, int beginF, int endF, float radius);

	bool IsBoneColEnable() const { return mBoneCollision.enable; }
};