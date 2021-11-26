#pragma once
#include <memory>

#include "lib/CollisionStructs.h"
#include "lib/Matrix.h"
#include "lib/SkinnedMesh.h"
#include "lib/Vector.h"
#include "lib/Primitive3D.h"


// �L�����N�^�[�̊��N���X
class Character
{
	// �萔
	static constexpr float DEFAULT_MOTION_BLEND_SPEED = 0.15f; // ���[�V�����u�����h�W��

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

private: // �ϐ�
	std::shared_ptr<SkinnedMesh> mMesh = nullptr;	// ���b�V��
	//std::shared_ptr<Primitive3D> mHit = nullptr;
	//std::unique_ptr<Primitive3D> mCol = nullptr;
	Matrix mWorld = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 }; // ���[���h�s��
	int mObjID = -1; // �C���X�^���X�Ɋ��蓖�Ă���ID(�eManager��0�X�^�[�g�ő����Ă���)
	int mCharaID = -1; // �L�����N�^�[���̂��̂Ɋ��蓖�Ă��Ă���ID(�X�^�[�g�͎�ނɂ���ĕς��ADataBase�Q��)
	CAPSULE mCapsuleParam; // �J�v�Z���̃p�����[�^

protected:
	Vector3 mPos = { 0,0,0 };	   // ���W
	Vector3 mVelocity = { 0,0,0 }; // ���x
	Vector3 mScale = { 1, 1, 1 };  // �X�P�[��
	Vector3 mAngle = { 0,0,0 };	   // �p�x
	float mMass = 0;			   //����
	bool mExist = true;			   // ���݃t���O
	Type mType = NONE;			   // �L�����N�^�[�^�C�v
	Vector3 mTargetPos = {};	   // �J����������ʒu
	Vector3 mTopPos = {};		   // �L�����̈�ԏ�̍��W

	BoneCollision mBoneCollision; // �{�[���̓����蔻��̃p�����[�^

public: // �֐�
	
	// �R���X�g���N�^
	Character(int charaID, Type type);

	// �f�X�g���N�^
	Character(const Character* org);
	virtual ~Character() = default;

	// ���[�V�����A���[���h�s��X�V
	void UpdateWorld();

	// �`��
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);

	// Velocity�����̊p�x�␳
	void CorrectionAngle();	

	// ���������̊p�x�␳
	void CorrectionAngle(const Vector3& dirN);

	// �J�v�Z���̃p�����[�^�ݒ�
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
	Vector3 GetFront() const { return -Vector3(mWorld._13, mWorld._23, mWorld._33); }
	Vector3 GetFrontXZ() const { return -Vector3(mWorld._13, 0.0f, mWorld._33).GetNormalize(); }
	Vector3 GetRight() const { return Vector3(mWorld._11, mWorld._21, mWorld._31); }
	Vector3 GetRightXZ() const { return Vector3(mWorld._11, 0.0f, mWorld._31).GetNormalize(); }
	Vector3 GetTargetPos() const { return mTargetPos; }
	Vector3 GetTopPos() const { return mTopPos; }
	float GetHeight() const { return (mMesh->mAABB.max.y * mScale.y - mMesh->mAABB.min.y * mScale.y); }
	Matrix GetWorld() const { return mWorld; }

	// �Z�b�^�[
	void SetPos(const Vector3& p) { mPos = p; }
	void SetVelocity(const Vector3 & v) { mVelocity = v; }
	void SetScale(const Vector3 & s) { mScale = s; }
	void SetAngle(const Vector3 & a) { mAngle = a; }
	void SetAngleY(float y) { mAngle.y = y; }
	void SetObjID(const int& i) { mObjID = i; }
	void SetCharaID(const int& i) { mCharaID = i; }
	void SetExist(const bool e) { mExist = e; }
	void SetMass(const float m) { mMass = m; }

	// ���[�V�����֘A
	// ���[�V�����̃^�C�v���擾
	MotionType GetMotion() const { return (MotionType)mMesh->GetMotion(); }
	// ���[�V������ݒ�
	void SetMotion(MotionType type, float blendSpeed = DEFAULT_MOTION_BLEND_SPEED) { mMesh->SetMotion(type, blendSpeed); }
	// 1�x�����Đ����郂�[�V�����Ƃ��̎��̃��[�V������ݒ�
	void SetMotionOnce(MotionType type, MotionType nextType, float blendSpeed = DEFAULT_MOTION_BLEND_SPEED) { mMesh->SetMotionOnce(type, nextType, blendSpeed); }
	// �Ō�̃t���[���Œ�~���郂�[�V������ݒ�
	void SetMotionStopLastFrame(MotionType type, float blendSpeed = DEFAULT_MOTION_BLEND_SPEED) { mMesh->SetMotionStopLastFrame(type, blendSpeed); }
	// ���[�V�����I�����ǂ������擾
	bool IsMotionFinished() const { return mMesh->IsMotionFinished(); }
	// ���[�V�����̃t���[�����擾
	int GetMotionFrame() const { return mMesh->GetFrame(); }
	// �{�[���s����擾
	Matrix GetBoneMatrix(); 
	// �{�[���̓����蔻����擾
	void GetBoneCollisionParam(Matrix* mat, float* radius);
	// �{�[���̓����蔻���ݒ�
	void SetBoneCollision(const char* boneName, int beginF, int endF, float radius);
	// �{�[���̓����蔻�肪�L�����擾
	bool IsBoneColEnable() const { return mBoneCollision.enable; }
};