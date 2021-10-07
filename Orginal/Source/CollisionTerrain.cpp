#include "CollisionTerrain.h"

#include "Terrain.h"

// staticメンバ変数
std::vector<Terrain*> CollisionTerrain::mTerrains;

float CollisionTerrain::GetHeight(const Vector3& pos, const float dist)
{
    Vector3 outPos;
    Vector3 outNormal;
    const Vector3 vel(0, -1, 0);
    for (auto& t : mTerrains)
    {
        int n = t->RayPickCol(pos + Vector3(0, dist, 0), vel, &outPos, &outNormal);
        if (n != -1) return outPos.y;
    }

    return pos.y;
}

int CollisionTerrain::RayPickOrg(const Vector3& sp, const Vector3& velocity, Vector3* outPos, Vector3* outNormal)
{
    int materialIndex = -1;
    for (auto& t : mTerrains)
    {
        materialIndex = t->RayPickOrg(sp, velocity, outPos, outNormal);
        if (materialIndex != -1) return materialIndex;
    }

    return materialIndex;
}

int CollisionTerrain::RayPick(const Vector3& sp, const Vector3& velocity, Vector3* outPos, Vector3* outNormal)
{
    int materialIndex = -1;
    for (auto& t : mTerrains)
    {
        materialIndex = t->RayPickCol(sp, velocity, outPos, outNormal);
        if (materialIndex != -1) return materialIndex;
    }

    return materialIndex;
}

bool CollisionTerrain::MoveCheck(const Vector3& pos, const Vector3& velocity, const float size, Vector3* outVelocity)
{
    *outVelocity = velocity;

    Vector3 cp;
    Vector3 normal;
    int n = RayPick(pos, *outVelocity, &cp, &normal);

    // もし壁に当たっていたら
    if (n != -1)
    {
        // サイズより距離が短かったら
        Vector3 dist = cp - pos;
        if (dist.Length() <= size)
        {
            // 戻し量計算
            float dot = Vector3::Dot(velocity, -normal);
            *outVelocity += normal * dot; // 壁から秒速dotではなれる

            // 新しい方向で壁チェック
            n = RayPick(pos, *outVelocity, &cp, &normal);
            if (n != 1)
            {
                // 壁があった場合は動かない
                dist = cp - pos;
                if (dist.Length() <= size)
                {
                    *outVelocity = Vector3::ZERO;
                }
            }

        }

        return true;
    }


    return false;
}

//int CollisionTerrain::MoveCheck(const Vector3& sp, const Vector3& velocity, Vector3* outPos)
//{
//	// レイピック
//	Vector3 hitPos, hitNormal;
//	float len = 0.0f;
//	int faceNumber = RayPick(sp, velocity, &hitPos, &hitNormal);
//	if (faceNumber == -1)
//	{
//		// ヒットしなかったら移動後の位置は終点
//		*outPos = ep;
//		return -1;
//	}
//
//	// 壁を突き抜けたベクトル
//	DirectX::XMVECTOR start = DirectX::XMLoadFloat3(&hitPos);
//	DirectX::XMVECTOR end = DirectX::XMLoadFloat3(&ep);
//	DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(end, start);
//
//	// 壁の法線ベクトルを単位化
//	DirectX::XMVECTOR normal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&hitNormal));
//
//	// 入射ベクトルを法線に射影
//	DirectX::XMVECTOR dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(vec), normal);
//
//	// 補正位置の計算
//	DirectX::XMVECTOR fixPos = DirectX::XMVectorMultiplyAdd(normal, dot, end);
//	DirectX::XMStoreFloat3(outPos, fixPos);
//
//    // 補正後の位置が壁にめり込んでいた場合は移動しないようにする
//	// 終点から補正位置のベクトルがメッシュに当たらなかったら
//	if (RayPick(*outPos, ep, &hitPos, &hitNormal) == -1)
//	{
//		*outPos = sp;
//	}
//
//	return faceNumber;
//
//}

bool CollisionTerrain::RegisterTerrain(Terrain* t)
{
    if (!t) return false;

    // 登録済みなら登録しない
    for (size_t i = 0; i < mTerrains.size(); ++i)
    {
        if (mTerrains[i] == t) return false;
        if (mTerrains[i]->GetID() == t->GetID()) return false;
    }

    mTerrains.push_back(t);
    return true;
}

bool CollisionTerrain::UnRegisterTerrain(Terrain* t)
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