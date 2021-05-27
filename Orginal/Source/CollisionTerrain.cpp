#include "CollisionTerrain.h"

#include "Terrain.h"

// static�����o�ϐ�
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
	// ���C�s�b�N
	Vector3 hitPos, hitNormal;
	float len = 0.0f;
	int faceNumber = RayPick(sp, ep, &hitPos, &hitNormal, &len);
	if (faceNumber == -1)
	{
		// �q�b�g���Ȃ�������ړ���̈ʒu�͏I�_
		*outPos = ep;
		return -1;
	}

	// �ǂ�˂��������x�N�g��
	DirectX::XMVECTOR start = DirectX::XMLoadFloat3(&hitPos);
	DirectX::XMVECTOR end = DirectX::XMLoadFloat3(&ep);
	DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(end, start);

	// �ǂ̖@���x�N�g����P�ʉ�
	DirectX::XMVECTOR normal = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&hitNormal));

	// ���˃x�N�g����@���Ɏˉe
	DirectX::XMVECTOR dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(vec), normal);

	// �␳�ʒu�̌v�Z
	DirectX::XMVECTOR fixPos = DirectX::XMVectorMultiplyAdd(normal, dot, end);
	DirectX::XMStoreFloat3(outPos, fixPos);

    // �␳��̈ʒu���ǂɂ߂荞��ł����ꍇ�͈ړ����Ȃ��悤�ɂ���
	// �I�_����␳�ʒu�̃x�N�g�������b�V���ɓ�����Ȃ�������
	//if (RayPick(*outPos, ep, &hitPos, &hitNormal, &len) == -1)
	//{
	//	*outPos = sp;
	//}

	return faceNumber;

}

bool CollisionTerrain::RegisterTerrain(const std::shared_ptr<Terrain>& t)
{
    if (!t) return false;

    // �o�^�ς݂Ȃ�o�^���Ȃ�
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