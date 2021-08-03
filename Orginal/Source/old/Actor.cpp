#include "Actor.h"

#include "CharacterManager.h"
#include "GameManager.h"
#include "MeshManager.h"
#include "Singleton.h"

Actor::Actor(int charaID, Type type)
{
	mMesh = Singleton<MeshManager>::GetInstance().SearchLoad(charaID);

	const Vector3& min = mMesh->mAABB.min;
	const Vector3& max = mMesh->mAABB.max;
	float radius = (max.z - min.z) * 0.5f;
	float x = min.x + ((max.x - min.x) * 0.5f);
	float z = min.z + ((max.z - min.z) * 0.5f);
	Vector3 cTop(x, max.y - radius, z);
	Vector3 cBottom(x, min.y + radius, z);

	mCapsuleParam.centerTop = cTop;
	mCapsuleParam.centerBottom = cBottom;
	mCapsuleParam.radius = radius;
	mHit = std::make_shared<GeometricCapsule>(cTop, cBottom, mCapsuleParam.radius);
	mCharaID = charaID;
	mType = type;
}

Actor::Actor(const Actor* org, Type type)
{
	// ���f�������ꏏ�Ɏg��
	mMesh = org->mMesh;
	mHit = org->mHit;
	mPos = Vector3(0, 0, 0);
	mScale = org->mScale;
	mAngle = Vector3(0, 0, 0);

	mCharaID = org->mCharaID;
	mType = type;

	mCapsuleParam = org->mCapsuleParam;

	UpdateWorld();
}

void Actor::UpdateWorld()
{
	// �e��X�V�̌�ɌĂяo��


	// ���[�V����������Ȃ�skinning
	if (mMesh->GetMotion() != SkinnedMesh::DEFAULT)
	{
		mMesh->Skinning();
	}

	{
		// ���t���[���̃��[���h�s����v�Z

		DirectX::XMMATRIX S, R, T;

		// �X�P�[��
		S = DirectX::XMMatrixScaling(mScale.x, mScale.y, mScale.z);

		// ��]
		R = DirectX::XMMatrixRotationRollPitchYaw(mAngle.x, mAngle.y, mAngle.z);

		// �ړ�
		T = DirectX::XMMatrixTranslation(mPos.x, mPos.y, mPos.z);

		DirectX::XMMATRIX world = S * R * T;

		// ���t���[���̃��[���h�s�����
		DirectX::XMStoreFloat4x4(&mWorld, world);
	}
}

void Actor::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	if (mExist)
	{
		DirectX::XMFLOAT4X4 wvp;
		DirectX::XMStoreFloat4x4(&wvp, DirectX::XMLoadFloat4x4(&mWorld) * DirectX::XMLoadFloat4x4(&view) * DirectX::XMLoadFloat4x4(&projection));
		mMesh->Render(wvp, mWorld, lightDir, GameManager::elpsedTime);
		//mHit->Render(wvp, mWorld, lightDir, DirectX::XMFLOAT4(0.8f, 0.2f, 0.2f, 0.2f));
	}
}

void Actor::Render(const Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDir)
{
	if (mExist)
	{
		DirectX::XMFLOAT4X4 wvp;
		DirectX::XMStoreFloat4x4(&wvp, DirectX::XMLoadFloat4x4(&mWorld) * DirectX::XMLoadFloat4x4(&view) * DirectX::XMLoadFloat4x4(&projection));
		mMesh->Render(shader, wvp, mWorld, lightDir, GameManager::elpsedTime);
	}
}

int Actor::RayPickSRT(const DirectX::XMFLOAT3& sp, const DirectX::XMFLOAT3& ep, DirectX::XMFLOAT3* outPos, DirectX::XMFLOAT3* outNormal, float* outLen)
{
	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&mWorld);
	DirectX::XMMATRIX worldInverse = DirectX::XMMatrixInverse(nullptr, world);

	// �O��̃I�u�W�F�N�g��Ԃł̃��C�ɕϊ�
	DirectX::XMVECTOR vSp = DirectX::XMLoadFloat3(&sp);
	DirectX::XMVECTOR vEp = DirectX::XMLoadFloat3(&ep);
	vSp = DirectX::XMVector3TransformCoord(vSp, worldInverse);
	vEp = DirectX::XMVector3TransformCoord(vEp, worldInverse);

	// ���C�s�b�N
	DirectX::XMFLOAT3 fSp, fEp;
	DirectX::XMStoreFloat3(&fSp, vSp);
	DirectX::XMStoreFloat3(&fEp, vEp);

	float len = 0.0f;
	int ret = -1;
	ret = mMesh->RayPick(fSp, fEp, outPos, outNormal, &len);

	if (ret != -1)
	{
		// �I�u�W�F�N�g��Ԃ��猻�݂̃��[���h��Ԃ֕ϊ�
		DirectX::XMVECTOR op = DirectX::XMLoadFloat3(outPos);
		DirectX::XMVECTOR on = DirectX::XMLoadFloat3(outNormal);
		op = DirectX::XMVector3TransformCoord(op, world);
		on = DirectX::XMVector3TransformCoord(on, world);

		DirectX::XMStoreFloat3(outPos, op);
		DirectX::XMStoreFloat3(outNormal, on);
	}

	return ret;
}

void Actor::CorrectionAngle(const Vector3& distN)
{
	// ��������`�⊮�ɂ�����
	Vector3 nowDist = Vector3(sinf(mAngle.y), 0.0f, cosf(mAngle.y));							 // �������Ă����

	Vector3 cross = nowDist.Cross(distN);				 // ���ς̊p�x��360�x�o���̂Ɏg��

	float dot = nowDist.Dot(distN);
	float min = -1.0f, max = 1.0f;
	float angle = acosf(Math::Clamp(dot, min, max)); // �p�x���Z�o(-1 ~ 1�̊Ԃɓ���Ȃ���nan���Ԃ邩��clamp���Ă�(dot = 1.00000012�݂����ɂȂ��Ă�))
	if (cross.y < 0) angle = DirectX::XMConvertToRadians(360.0f) - angle;
	mAngle.y += angle;
}

void Actor::AddMotion(const char* filename, SkinnedMesh::MotionType type)
{
	mMesh->AddMotion(filename, type);
}

void Actor::SetMotion(SkinnedMesh::MotionType type, bool isLoop)
{
	mMesh->SetMotion(type, isLoop);
}

AABB Actor::GetLocalAABB() const
{
	Vector3 scale = GetScale();
	AABB aabb = mMesh->mAABB;
	aabb.min.x = aabb.min.x * scale.x;
	aabb.min.y = aabb.min.y * scale.y;
	aabb.min.z = aabb.min.z * scale.z;
	aabb.max.x = aabb.max.x * scale.x;
	aabb.max.y = aabb.max.y * scale.y;
	aabb.max.z = aabb.max.z * scale.z;

	return aabb;
}

AABB Actor::GetAABB() const
{
	Vector3 pos = GetPos();
	Vector3 scale = GetScale();
	AABB aabb = mMesh->mAABB;
	aabb.min.x = aabb.min.x * scale.x + pos.x;
	aabb.min.y = aabb.min.y * scale.y + pos.y;
	aabb.min.z = aabb.min.z * scale.z + pos.z;
	aabb.max.x = aabb.max.x * scale.x + pos.x;
	aabb.max.y = aabb.max.y * scale.y + pos.y;
	aabb.max.z = aabb.max.z * scale.z + pos.z;
	return aabb;
}

CAPSULE Actor::GetCapsule() const
{
	Vector3 pos = GetPos();
	Vector3 scale = GetScale();
	CAPSULE c = mCapsuleParam;
	c.centerTop.x = c.centerTop.x * scale.x + pos.x;
	c.centerTop.y = c.centerTop.y * scale.y + pos.y;
	c.centerTop.z = c.centerTop.z * scale.z + pos.z;
	c.centerBottom.x = c.centerBottom.x * scale.x + pos.x;
	c.centerBottom.y = c.centerBottom.y * scale.y + pos.y;
	c.centerBottom.z = c.centerBottom.z * scale.z + pos.z;
	c.radius = c.radius * scale.z;
	return c;
}
