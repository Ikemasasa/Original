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
#include "Singleton.h"
#include "TransformData.h"
#include "MotionCollision.h"

Player::Player(int charaID) : Character(charaID, Character::PLAYER)
{
	// �Ƃ肠�����A�C�e���������n��
	for (int i = 0; i < 2; ++i)
	{
		mInventory.Add(ItemData::PORTION);
		mInventory.Add(ItemData::MAGIC_PORTION);
		mInventory.Add(ItemData::BOMB);
	}

	// ���ʐݒ�
	mMass = MASS;
}

void Player::Initialize()
{
	// �g�����X�t�H�[���擾
	TransformData::Transform transform = TransformData::GetPLTransform(GetCharaID());
	mPos   = transform.pos;
	mScale = transform.scale;
	mAngle = transform.angle;

	// �J�v�Z���̃p�����[�^�ݒ�
	SetCapsuleParam(transform.diameter / 2.0f);

	// �{�[���̓����蔻��ݒ�
	MotionCollision::ColData colData = MotionCollision::GetPLMotionCollision(GetCharaID());
	SetBoneCollision(colData.boneName.c_str(), colData.beginFrame, colData.endFrame, colData.radius);

	// ���[�V�����̐ݒ�
	SetMotion(Character::IDLE);
}

void Player::Update()
{
	// �J�����̃x�N�g���擾
	auto& cameraManager = Singleton<CameraManager>().GetInstance();
	Vector3 cameraForward = cameraManager.GetForwardXZ();
	Vector3 cameraRight = cameraManager.GetRightXZ();

	// ���͏����擾
	float axisX = Input::GetAxisX();
	float axisY = Input::GetAxisY();

	// ���͂������
	if (axisX != 0 || axisY != 0)
	{
		// �������Ȃ������߁A��̂͑���ɂ��Ă���
		mVelocity = cameraForward * axisY;
		mVelocity += cameraRight * axisX;
		mVelocity *= RUN_SPEED;

		// �����␳
		CorrectionAngle();
		// ���W�␳
		{
			// �ړ�����
			Vector3 outVelocity;
			if (CollisionTerrain::MoveCheck(mTargetPos, mVelocity, GetCapsule().radius, &outVelocity))
			{
				mVelocity = outVelocity;
			}
			//mPos += mVelocity;
			mPos.x += mVelocity.x;
			mPos.z += mVelocity.z;

			// �������̕␳
			float y = CollisionTerrain::GetHeight(mPos, RAY_DOWN_DIST);
			mPos.y = Math::Lerp(mPos.y, y, POS_Y_LERP_FACTOR);
		}

		SetMotion(Character::RUN);
	}
	else
	{
		SetMotion(Character::IDLE);
	}

	Character::UpdateWorld();
}