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
	// とりあえずアイテムを何個か渡す
	for (int i = 0; i < 2; ++i)
	{
		mInventory.Add(ItemData::PORTION);
		mInventory.Add(ItemData::MAGIC_PORTION);
		mInventory.Add(ItemData::BOMB);
	}

	// 質量設定
	mMass = MASS;
}

void Player::Initialize()
{
	// トランスフォーム取得
	TransformData::Transform transform = TransformData::GetPLTransform(GetCharaID());
	mPos   = transform.pos;
	mScale = transform.scale;
	mAngle = transform.angle;

	// カプセルのパラメータ設定
	SetCapsuleParam(transform.diameter / 2.0f);

	// ボーンの当たり判定設定
	MotionCollision::ColData colData = MotionCollision::GetPLMotionCollision(GetCharaID());
	SetBoneCollision(colData.boneName.c_str(), colData.beginFrame, colData.endFrame, colData.radius);

	// モーションの設定
	SetMotion(Character::IDLE);
}

void Player::Update()
{
	// カメラのベクトル取得
	auto& cameraManager = Singleton<CameraManager>().GetInstance();
	Vector3 cameraForward = cameraManager.GetForwardXZ();
	Vector3 cameraRight = cameraManager.GetRightXZ();

	// 入力情報を取得
	float axisX = Input::GetAxisX();
	float axisY = Input::GetAxisY();

	// 入力があれば
	if (axisX != 0 || axisY != 0)
	{
		// 慣性をなくすため、上のは代入にしてある
		mVelocity = cameraForward * axisY;
		mVelocity += cameraRight * axisX;
		mVelocity *= RUN_SPEED;

		// 向き補正
		CorrectionAngle();
		// 座標補正
		{
			// 移動方向
			Vector3 outVelocity;
			if (CollisionTerrain::MoveCheck(mTargetPos, mVelocity, GetCapsule().radius, &outVelocity))
			{
				mVelocity = outVelocity;
			}
			//mPos += mVelocity;
			mPos.x += mVelocity.x;
			mPos.z += mVelocity.z;

			// 下方向の補正
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