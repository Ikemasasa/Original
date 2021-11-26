#pragma once

#include <memory>

#include "lib/Vector.h"
#include "lib/Matrix.h"

class SkinnedMesh;
class Shader;

// 動かないメッシュを扱うクラス(モーションの更新とかをしていない)
class Object
{
private: // 変数

	std::shared_ptr<SkinnedMesh> mMesh = nullptr;	 // オリジナルのメッシュ(描画用)
	std::shared_ptr<SkinnedMesh> mMeshLow = nullptr; // ロウポリのメッシュ
	std::shared_ptr<SkinnedMesh> mMeshCol = nullptr; // コリジョン用のメッシュ
	Matrix mWorld = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 }; // ワールド行列
	int mID = -1; // ID

protected:
	Vector3 mPos = { 0,0,0 };     // 座標
	Vector3 mScale = { 1, 1, 1 }; // スケール
	Vector3 mAngle = { 0,0,0 };	  // 角度

public: // 関数

	// コンストラクタ
	Object(int id);
	Object(const Object* obj);

	// デストラクタ
	~Object() = default;

	// ワールド行列を更新する関数
	void UpdateWorld();

	// オリジナルメッシュを描画
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);

	// ロウポリメッシュを描画
	void RenderLow(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);

	// ロウポリのメッシュにRayを飛ばす(Orgっていうのは安心するためのウソ)
	int RayPickOrg(const Vector3& pos, const Vector3& velocity, Vector3* outPos, Vector3* outNormal);

	// コリジョン用のメッシュにRayを飛ばす
	int RayPickCol(const Vector3& pos, const Vector3& velocity, Vector3* outPos, Vector3* outNormal);

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