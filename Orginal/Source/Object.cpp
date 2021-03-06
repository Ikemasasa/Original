#include "Object.h"

#include "GameManager.h"
#include "MeshManager.h"
#include "Singleton.h"


Object::Object(int id)
{
	mMesh = Singleton<MeshManager>().GetInstance().SearchLoad(id);
	mMeshLow = Singleton<MeshManager>().GetInstance().SearchLoad(id + 1);
	mMeshCol = Singleton<MeshManager>().GetInstance().SearchLoad(id + 2);
	mID = id;

	UpdateWorld();
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
	mWorld.SRT(mScale, mAngle, mPos);
}

void Object::Render(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir)
{
	Matrix wvp = mWorld * view * proj;
	mMesh->Render(shader, wvp, mWorld, lightDir);
}

void Object::RenderLow(const Shader* shader, const Matrix& view, const Matrix& proj, const Vector4& lightDir)
{
	Matrix wvp = mWorld * view * proj;
	mMeshLow->Render(shader, wvp, mWorld, lightDir);
}

int Object::RayPickOrg(const Vector3& pos, const Vector3& velocity, Vector3* outPos, Vector3* outNormal)
{
	// ワールド行列の逆行列作成
	Matrix worldInv = mWorld;
	worldInv.Inverse();

	// 座標を逆変換
	Vector3 posInv = Vector3::TransformCoord(pos, worldInv);
	Vector3 velInv = Vector3::TransformCoord(velocity, worldInv);

	int ret = mMeshLow->RayPick(posInv, velInv, outPos, outNormal);
	if (ret != -1)
	{
		// 結果を現状に合わせる
		*outPos = Vector3::TransformCoord(*outPos, mWorld);
		*outNormal = Vector3::TransformCoord(*outNormal, mWorld);
	}

	return ret;
}

int Object::RayPickCol(const Vector3& pos, const Vector3& velocity, Vector3* outPos, Vector3* outNormal)
{
	// ワールド行列の逆行列作成
	Matrix worldInv = mWorld;
	worldInv.Inverse();

	// 座標を逆変換
	Vector3 posInv = Vector3::TransformCoord(pos, worldInv);
	Vector3 velInv = Vector3::TransformCoord(velocity, worldInv);

	int ret = mMeshCol->RayPick(posInv, velInv, outPos, outNormal);
	if (ret != -1)
	{
		// 結果を現状に合わせる
		*outPos = Vector3::TransformCoord(*outPos, mWorld);
		*outNormal = Vector3::TransformCoord(*outNormal, mWorld);
	}

	return ret;
}