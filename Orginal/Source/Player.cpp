#include "Player.h"

#include <directxmath.h>

#include "lib/Input.h"
#include "lib/Math.h"

#include "CameraManager.h"
#include "CollisionTerrain.h"
#include "Define.h"
#include "GameManager.h"
#include "ItemData.h"
#include "StatusData.h"
#include "TransformData.h"

Player::Player(int charaID) : Character(charaID, Character::PLAYER)
{
	// �V�F�[�_��������
	Shader* shader = new Shader;
	shader->Load(L"Shaders/Character.fx", "VSMain", "PSMain");
	ChangeShader(shader);

	// �Ƃ肠�����A�C�e���������n��
	for (int i = 0; i < 2; ++i)
	{
		mInventory.Add(ItemData::PORTION);
		mInventory.Add(ItemData::MAGIC_PORTION);
		mInventory.Add(ItemData::BOMB);
	}

	mMass = MASS;
}

void Player::Initialize()
{
	TransformData::Transform transform = TransformData::GetPLTransform(GetCharaID());
	mPos   = transform.pos;
	mScale = transform.scale;
	mAngle = transform.angle;

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
		// �����␳
		CorrectionAngle();
		// ���W�␳
		{
			const float RAYPICK_DIST = 0.5f;

			// �ړ�����
			Vector3 outVelocity;
			if (CollisionTerrain::MoveCheck(mPos + Vector3(0, RAYPICK_DIST, 0), mVelocity, GetCapsule().radius, &outVelocity))
			{
				mVelocity = outVelocity;
			}
			mPos += mVelocity;


			// ������
			mPos.y = CollisionTerrain::GetHeight(mPos, RAYPICK_DIST);
		}

		SetMotion(SkinnedMesh::RUN);
	}
	else
	{
		SetMotion(SkinnedMesh::IDLE);
	}

	Character::UpdateWorld();
}