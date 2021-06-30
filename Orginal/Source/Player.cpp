#include "Player.h"

#include <directxmath.h>

#include "CameraManager.h"
#include "CollisionTerrain.h"
#include "Define.h"
#include "DataBase.h"
#include "ItemData.h"
#include "Singleton.h"
#include "StatusData.h"


#include "lib/GeometricPrimitive.h"
#include "lib/Input.h"
#include "lib/Math.h"

#include "lib/Font.h"

Player::Player(int charaID) : Actor(charaID, Actor::PLAYER)
{
	SetCharaID(charaID);
	mStatus = Singleton<DataBase>::GetInstance().GetStatusData()->GetPLStatus(charaID);
	mVelocity = Vector3::Zero();

	Shader* shader = new Shader;
	shader->Load(L"Shaders/Character.fx", "VSMain", "PSMain");
	ChangeShader(shader);

	// �e�X�g
	for (int i = 0; i < 3; ++i)
	{
		mInventory.Add(Singleton<DataBase>().GetInstance().GetItemData()->GetItemParam(ItemData::PORTION));
		mInventory.Add(Singleton<DataBase>().GetInstance().GetItemData()->GetItemParam(ItemData::MAGIC_PORTION));
		mInventory.Add(Singleton<DataBase>().GetInstance().GetItemData()->GetItemParam(ItemData::BOMB));
	}
}

void Player::Initialize()
{
	SetPos(Vector3(10.0f, 0.0f, 10.0f));
	SetScale(Vector3(0.02f, 0.02f, 0.02f));
}

void Player::Update()
{
	auto& cameraManager = Singleton<CameraManager>().GetInstance();
	Vector3 cFrontVector(cameraManager.GetFrontVector().x, 0.0f, cameraManager.GetFrontVector().z); // �J�����̐��ʂ̃x�N�g��(XZ)
	Vector3 cRightVector(cameraManager.GetRightVector().x, 0.0f, cameraManager.GetRightVector().z); // �J�����̉E���̃x�N�g��(XZ)
	cFrontVector.Normalize();
	cRightVector.Normalize();

	const float MOVE_SPEED = 0.3f;
	float angleY = GetAngle().y;
	Vector3 oldPos = GetPos();
	Vector3 movedPos = {};

	// �����_
	auto CalcVelocity = [&]()
	{
		Vector3 velocity = {};
		velocity += cFrontVector * Input::GetAxisY() * MOVE_SPEED;
		velocity += cRightVector * Input::GetAxisX() * MOVE_SPEED;
		mVelocity = velocity;

		movedPos = oldPos + mVelocity;
	};
	auto CorrectionAngle = [&]()
	{
		// ��������`�⊮�ɂ�����
		Vector3 vec0 = cFrontVector;							 // 0�x�ƂȂ�x�N�g��
		Vector3 dist = Vector3(mVelocity.x, 0.0f, mVelocity.z);	 // �s�����������̃x�N�g��
		dist.Normalize();

		Vector3 cross = dist.Cross(cFrontVector);				 // ���ς̊p�x��360�x�o���̂Ɏg��

		float dot = dist.Dot(vec0);
		float min = -1.0f, max = 1.0f;
		float a = acosf(Math::Clamp(dot, min, max)); // �p�x���Z�o(-1 ~ 1�̊Ԃɓ���Ȃ���nan���Ԃ邩��clamp���Ă�(dot = 1.00000012�݂����ɂȂ��Ă�))
		if (cross.y < 0) a = DirectX::XMConvertToRadians(360.0f) - a;
		angleY = cameraManager.GetAngle().y - a - DirectX::XMConvertToRadians(180.0f);
	};
	auto CorrectionPos = [&]()
	{
		const float RAYPICK_OFFSET = 0.25f;
		Vector3 sp = Vector3(movedPos.x, movedPos.y + RAYPICK_OFFSET, movedPos.z);
		Vector3 ep = Vector3(movedPos.x, movedPos.y - RAYPICK_OFFSET, movedPos.z);
		Vector3 outPos, outNormal;
		float len;

		// ������
		if (-1 != CollisionTerrain::RayPick(sp, ep, &outPos, &outNormal, &len))
		{
			movedPos.y = outPos.y;
		}

		// �ړ�����
		sp = Vector3(oldPos.x, oldPos.y + RAYPICK_OFFSET, oldPos.z); // �ړ��O�̍��W
		ep = Vector3(movedPos.x, movedPos.y + RAYPICK_OFFSET, movedPos.z);
		if (-1 != CollisionTerrain::MoveCheck(sp, ep, &outPos))
		{
			movedPos.x = outPos.x;
			movedPos.z = outPos.z;
		}
	};

	CalcVelocity();
	if (mVelocity.x || mVelocity.z) // �ړ�����Ȃ�
	{
		CorrectionAngle();
		CorrectionPos();

		SetMotion(SkinnedMesh::RUN);
	}
	else
	{
		SetMotion(SkinnedMesh::IDLE);
	}

	SetPos(movedPos);
	SetAngleY(angleY);

	Actor::UpdateWorld();
}