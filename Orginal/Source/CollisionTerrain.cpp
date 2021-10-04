#include "CollisionTerrain.h"

#include "Terrain.h"

// static�����o�ϐ�
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

    // �����ǂɓ������Ă�����
    if (n != -1)
    {
        // �T�C�Y��苗�����Z��������
        Vector3 dist = cp - pos;
        if (dist.Length() <= size)
        {
            // �߂��ʌv�Z
            float dot = Vector3::Dot(velocity, -normal);
            *outVelocity += normal * dot; // �ǂ���b��dot�ł͂Ȃ��

            // �V���������ŕǃ`�F�b�N
            n = RayPick(pos, *outVelocity, &cp, &normal);
            if (n != 1)
            {
                // �ǂ��������ꍇ�͓����Ȃ�
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
//	// ���C�s�b�N
//	Vector3 hitPos, hitNormal;
//	float len = 0.0f;
//	int faceNumber = RayPick(sp, velocity, &hitPos, &hitNormal);
//	if (faceNumber == -1)
//	{
//		// �q�b�g���Ȃ�������ړ���̈ʒu�͏I�_
//		*outPos = ep;
//		return -1;
//	}
//
//	// �ǂ�˂��������x�N�g��
//	DirectX::XMVECTOR start = DirectX::XMLoadFloat3(&hitPos);
//	DirectX::XMVECTOR end = DirectX::XMLoadFloat3(&ep);
//	DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(end, start);
//
//	// �ǂ̖@���x�N�g����P�ʉ�
//	DirectX::XMVECTOR normal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&hitNormal));
//
//	// ���˃x�N�g����@���Ɏˉe
//	DirectX::XMVECTOR dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(vec), normal);
//
//	// �␳�ʒu�̌v�Z
//	DirectX::XMVECTOR fixPos = DirectX::XMVectorMultiplyAdd(normal, dot, end);
//	DirectX::XMStoreFloat3(outPos, fixPos);
//
//    // �␳��̈ʒu���ǂɂ߂荞��ł����ꍇ�͈ړ����Ȃ��悤�ɂ���
//	// �I�_����␳�ʒu�̃x�N�g�������b�V���ɓ�����Ȃ�������
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

    // �o�^�ς݂Ȃ�o�^���Ȃ�
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