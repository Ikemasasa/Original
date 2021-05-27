#include "CollisionTerrain.h"

#include "Terrain.h"

// staticメンバ変数
std::vector<std::shared_ptr<Terrain>> CollisionTerrain::mTerrains;

int CollisionTerrain::RayPick(Vector3 sp, Vector3 ep, Vector3* outPos, Vector3* outNormal, float* outLen)
{
    int materialIndex = -1;
    for (auto& t : mTerrains)
    {
        materialIndex = t->RayPickSRT(sp, ep, outPos, outNormal, outLen);

        if (materialIndex != -1) return materialIndex;
    }

    return materialIndex;
}

int CollisionTerrain::MoveCheck(const Vector3& sp, const Vector3& ep, Vector3* outPos)
{
	// レイピック
	Vector3 hitPos, hitNormal;
	float len = 0.0f;
	int faceNumber = RayPick(sp, ep, &hitPos, &hitNormal, &len);
	if (faceNumber == -1)
	{
		// ヒットしなかったら移動後の位置は終点
		*outPos = ep;
		return -1;
	}

	// 壁を突き抜けたベクトル
	DirectX::XMVECTOR start = DirectX::XMLoadFloat3(&hitPos);
	DirectX::XMVECTOR end = DirectX::XMLoadFloat3(&ep);
	DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(end, start);

	// 壁の法線ベクトルを単位化
	DirectX::XMVECTOR normal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&hitNormal));

	// 入射ベクトルを法線に射影
	DirectX::XMVECTOR dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(vec), normal);

	// 補正位置の計算
	DirectX::XMVECTOR fixPos = DirectX::XMVectorMultiplyAdd(normal, dot, end);
	DirectX::XMStoreFloat3(outPos, fixPos);

    // 補正後の位置が壁にめり込んでいた場合は移動しないようにする
	// 終点から補正位置のベクトルがメッシュに当たらなかったら
	//if (RayPick(*outPos, ep, &hitPos, &hitNormal, &len) == -1)
	//{
	//	*outPos = sp;
	//}

	return faceNumber;

}

bool CollisionTerrain::RegisterTerrain(const std::shared_ptr<Terrain>& t)
{
    if (!t) return false;

    // 登録済みなら登録しない
    for (size_t i = 0; i < mTerrains.size(); ++i)
    {
        if (mTerrains[i] == t) return false;
    }

    mTerrains.emplace_back(t);
    return true;
}

bool CollisionTerrain::UnRegisterTerrain(const std::shared_ptr<Terrain>& t)
{
    for (size_t i = 0; i < mTerrains.size(); ++i)
    {
        if (mTerrains[i] == t)
        {
            mTerrains.erase(mTerrains.begin() + i);
            return  true;
        }
    }

    return false;
}

bool CollisionTerrain::UnRegisterTerrainAll()
{
    for (size_t i = 0; i < mTerrains.size(); ++i)
    {
        mTerrains.pop_back();
    }

    return true;
}