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

Player::Player(int charaID) : Character(charaID, Character::PLAYER)
{
	// シェーダ書き換え
	Shader* shader = new Shader;
	shader->Load(L"Shaders/Character.fx", "VSMain", "PSMain");
	ChangeShader(shader);

	// テスト
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
	// カメラのベクトル取得
	auto& cameraManager = Singleton<CameraManager>().GetInstance();
	Vector3 cFrontVector(cameraManager.GetFrontVector().x, 0.0f, cameraManager.GetFrontVector().z); // カメラの正面のベクトル(XZ)
	Vector3 cRightVector(cameraManager.GetRightVector().x, 0.0f, cameraManager.GetRightVector().z); // カメラの右側のベクトル(XZ)
	cFrontVector.Normalize();
	cRightVector.Normalize();

	float axisX = Input::GetAxisX();
	float axisY = Input::GetAxisY();
	if (axisX > 0 || axisY > 0)
	{
		mVelocity.x = cFrontVector.x * axisX * MOVE_SPEED;
		mVelocity.z = cFrontVector.z * axisY * MOVE_SPEED;
		mPos += mVelocity;

		// 向き補正
		CorrectionAngle();

		// 座標補正
		{
			// 下方向
			const float RAYPICK_DIST = 0.25f;
			Vector3 sp = Vector3(mPos.x, mPos.y + RAYPICK_DIST, mPos.z);
			Vector3 ep = Vector3(mPos.x, mPos.y - RAYPICK_DIST, mPos.z);
			Vector3 outPos, outNormal;
			float len;
			if (-1 != CollisionTerrain::RayPick(sp, ep, &outPos, &outNormal, &len))
			{
				mPos.y = outPos.y;
			}

			// 移動方向
			sp = mPos - mVelocity; // 移動前の座標
			ep = mPos;			   // 移動後の座標
			if (-1 != CollisionTerrain::MoveCheck(sp, ep, &outPos))
			{
				mPos.x = outPos.x;
				mPos.z = outPos.z;
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