#include "Player.h"

#include <directxmath.h>

#include "CameraManager.h"
#include "CollisionTerrain.h"
#include "Define.h"
#include "DataBase.h"
#include "GameManager.h"
#include "ItemData.h"
#include "Singleton.h"
#include "StatusData.h"

#include "lib/Input.h"
#include "lib/Math.h"

Player::Player(int charaID) : Character(charaID, Character::PLAYER)
{
	// �V�F�[�_��������
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
	mPos   = Vector3(10.0f, 0.0f, 10.0f);
	mScale = Vector3(0.02f, 0.02f, 0.02f);
	SetMotion(SkinnedMesh::IDLE);
}

void Player::Update()
{
	// �J�����̃x�N�g���擾
	auto& cameraManager = Singleton<CameraManager>().GetInstance();
	Vector3 cFrontVector(cameraManager.GetFrontVector().x, 0.0f, cameraManager.GetFrontVector().z); // �J�����̐��ʂ̃x�N�g��(XZ)
	Vector3 cRightVector(cameraManager.GetRightVector().x, cameraManager.GetRightVector().y, cameraManager.GetRightVector().z); // �J�����̉E���̃x�N�g��(XZ)
	cFrontVector.Normalize();
	cRightVector.Normalize();

	float axisX = Input::GetAxisX();
	float axisY = Input::GetAxisY();
	if (axisX != 0 || axisY != 0)
	{
		// �������Ȃ������߁A��̂͑���ɂ��Ă���
		mVelocity  = cFrontVector * axisY;
		mVelocity += cRightVector * axisX;
		mVelocity *= MOVE_SPEED;
		mPos += mVelocity;

		// �����␳
		CorrectionAngle();

		// ���W�␳
		{
			const float RAYPICK_DIST = 0.25f;
			Vector3 outPos, outNormal;

			// �ړ�����(y = pos���ƁA���܂����Ƃ�����Ȃ�����A������ɂ�����)
			Vector3 oldPos = mPos - mVelocity;
			Vector3 sp = Vector3(oldPos.x, oldPos.y + RAYPICK_DIST, oldPos.z); // �ړ��O�̍��W
			Vector3 ep = Vector3(mPos.x, mPos.y + RAYPICK_DIST, mPos.z);	   // �ړ���̍��W
			if (-1 != CollisionTerrain::MoveCheck(sp, ep, &outPos))
			{
				mPos.x = outPos.x;
				mPos.z = outPos.z;
			}

			// ������
			sp = Vector3(mPos.x, mPos.y + RAYPICK_DIST, mPos.z);
			ep = Vector3(mPos.x, mPos.y - RAYPICK_DIST, mPos.z);
			float len;
			if (-1 != CollisionTerrain::RayPick(sp, ep, &outPos, &outNormal, &len))
			{
				mPos.y = outPos.y;
			}


		}

		SetMotion(SkinnedMesh::RUN);
	}
	else
	{
		SetMotion(SkinnedMesh::IDLE);
	}

	Character::UpdateWorld();
}