#pragma once
#include <memory>

#include "lib/CollisionStructs.h"
#include "lib/Matrix.h"
#include "lib/SkinnedMesh.h"
#include "lib/Vector.h"
#include "lib/Primitive3D.h"


// キャラクターの基底クラス
class Character
{
	// 定数
	static constexpr float DEFAULT_MOTION_BLEND_SPEED = 0.15f; // モーションブレンド係数

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

private: // 変数
	std::shared_ptr<SkinnedMesh> mMesh = nullptr;	// メッシュ
	//std::shared_ptr<Primitive3D> mHit = nullptr;
	//std::unique_ptr<Primitive3D> mCol = nullptr;
	Matrix mWorld = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 }; // ワールド行列
	int mObjID = -1; // インスタンスに割り当てられるID(各Managerで0スタートで増えていく)
	int mCharaID = -1; // キャラクターそのものに割り当てられているID(スタートは種類によって変わる、DataBase参照)
	CAPSULE mCapsuleParam; // カプセルのパラメータ

protected:
	Vector3 mPos = { 0,0,0 };	   // 座標
	Vector3 mVelocity = { 0,0,0 }; // 速度
	Vector3 mScale = { 1, 1, 1 };  // スケール
	Vector3 mAngle = { 0,0,0 };	   // 角度
	float mMass = 0;			   //質量
	bool mExist = true;			   // 存在フラグ
	Type mType = NONE;			   // キャラクタータイプ
	Vector3 mTargetPos = {};	   // カメラが見る位置
	Vector3 mTopPos = {};		   // キャラの一番上の座標

	BoneCollision mBoneCollision; // ボーンの当たり判定のパラメータ

public: // 関数
	
	// コンストラクタ
	Character(int charaID, Type type);

	// デストラクタ
	Character(const Character* org);
	virtual ~Character() = default;

	// モーション、ワールド行列更新
	void UpdateWorld();

	// 描画
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);

	// Velocity方向の角度補正
	void CorrectionAngle();	

	// 引数方向の角度補正
	void CorrectionAngle(const Vector3& dirN);

	// カプセルのパラメータ設定
	void SetCapsuleParam(float radius);

	// モーション追加
	void AddMotion(const char* filename, MotionType type) { mMesh->AddMotion(filename, type); }


	//ゲッター
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

	// セッター
	void SetPos(const Vector3& p) { mPos = p; }
	void SetVelocity(const Vector3 & v) { mVelocity = v; }
	void SetScale(const Vector3 & s) { mScale = s; }
	void SetAngle(const Vector3 & a) { mAngle = a; }
	void SetAngleY(float y) { mAngle.y = y; }
	void SetObjID(const int& i) { mObjID = i; }
	void SetCharaID(const int& i) { mCharaID = i; }
	void SetExist(const bool e) { mExist = e; }
	void SetMass(const float m) { mMass = m; }

	// モーション関連
	// モーションのタイプを取得
	MotionType GetMotion() const { return (MotionType)mMesh->GetMotion(); }
	// モーションを設定
	void SetMotion(MotionType type, float blendSpeed = DEFAULT_MOTION_BLEND_SPEED) { mMesh->SetMotion(type, blendSpeed); }
	// 1度だけ再生するモーションとその次のモーションを設定
	void SetMotionOnce(MotionType type, MotionType nextType, float blendSpeed = DEFAULT_MOTION_BLEND_SPEED) { mMesh->SetMotionOnce(type, nextType, blendSpeed); }
	// 最後のフレームで停止するモーションを設定
	void SetMotionStopLastFrame(MotionType type, float blendSpeed = DEFAULT_MOTION_BLEND_SPEED) { mMesh->SetMotionStopLastFrame(type, blendSpeed); }
	// モーション終了かどうかを取得
	bool IsMotionFinished() const { return mMesh->IsMotionFinished(); }
	// モーションのフレームを取得
	int GetMotionFrame() const { return mMesh->GetFrame(); }
	// ボーン行列を取得
	Matrix GetBoneMatrix(); 
	// ボーンの当たり判定を取得
	void GetBoneCollisionParam(Matrix* mat, float* radius);
	// ボーンの当たり判定を設定
	void SetBoneCollision(const char* boneName, int beginF, int endF, float radius);
	// ボーンの当たり判定が有効か取得
	bool IsBoneColEnable() const { return mBoneCollision.enable; }
};