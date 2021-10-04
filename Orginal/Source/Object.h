#pragma once

#include <memory>

#include "lib/Vector.h"
#include "lib/Matrix.h"

class SkinnedMesh;
class Shader;

class Object
{
private:
	std::shared_ptr<SkinnedMesh> mMesh = nullptr;
	std::shared_ptr<SkinnedMesh> mMeshLow = nullptr;
	std::shared_ptr<SkinnedMesh> mMeshCol = nullptr;
	Matrix mWorld = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
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
	void Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);
	void RenderLow(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir);

	// レイピック
	int RayPickOrg(const Vector3& pos, const Vector3& velocity, Vector3* outPos, Vector3* outNormal);
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