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
#include "MotionCollision.h"

Player::Player(int charaID) : Character(charaID, Character::PLAYER)
{
	// とりあえずアイテムを何個か渡す
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

	MotionCollision::ColData colData = MotionCollision::GetPLMotionCollision(GetCharaID());
	SetBoneCollision(colData.boneName.c_str(), colData.beginFrame, colData.endFrame, colData.radius);

	SetMotion(Character::IDLE);
}

void Player::Update()
{
	// カメラのベクトル取得
	auto& cameraManager = Singleton<CameraManager>().GetInstance();
	Vector3 cFrontVector(cameraManager.GetFrontVector().x, 0.0f, cameraManager.GetFrontVector().z); // カメラの正面のベクトル(XZ)
	Vector3 cRightVector(cameraManager.GetRightVector().x, 0.0f, cameraManager.GetRightVector().z); // カメラの右側のベクトル(XZ)
	cFrontVector.Normalize();
	cRightVector.Normalize();

	static bool attack = false;

	if (!attack)
	{
		float axisX = Input::GetAxisX();
		float axisY = Input::GetAxisY();
		if (axisX != 0 || axisY != 0)
		{
			// 慣性をなくすため、上のは代入にしてある
			mVelocity = cFrontVector * axisY;
			mVelocity += cRightVector * axisX;
			mVelocity *= MOVE_SPEED;
			// 向き補正
			CorrectionAngle();
			// 座標補正
			{
				// 移動方向
				Vector3 outVelocity;
				if (CollisionTerrain::MoveCheck(mPos + Vector3(0, RAYPICK_DIST, 0), mVelocity, GetCapsule().radius, &outVelocity))
				{
					mVelocity = outVelocity;
				}
				//mPos += mVelocity;
				mPos.x += mVelocity.x;
				mPos.z += mVelocity.z;

				// 下方向の補正
				float y = CollisionTerrain::GetHeight(mPos, RAYPICK_DIST);
				mPos.y = Math::Lerp(mPos.y, y, POS_Y_ADJUST_FACTOR);
			}

			SetMotion(Character::RUN);
		}
		else
		{
			SetMotion(Character::IDLE);
		}
		
		if (Input::GetButtonTrigger(0, Input::A))
		{
			SetMotionOnce(ATTACK, IDLE);
			attack = true;
		}
	}
	else
	{
		if (IsMotionFinished()) attack = false;
	}


	Character::UpdateWorld();
}