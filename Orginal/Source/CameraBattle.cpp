#include "CameraBattle.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "Character.h"
#include "Define.h"
#include "GameManager.h"

CameraBattle::CameraBattle() : CameraBase()
{
	// 開始演出の座標決定
	const float BEGIN_POS_Y = 10.0f;
	mBeginPos.x = 0.0f;
	mBeginPos.y = BEGIN_POS_Y;
	mBeginPos.z = BattleCharacterManager::PLAYER_POS_Z * 2.0f;

	Vector3 dist = Vector3(0, 0, BattleCharacterManager::ENEMY_POS_Z) - mBeginPos;
	float distRate = 0.3f;
	mEndPos = mBeginPos + dist * distRate;
	
	const float BEGIN_TARGET_Y = 3.0f;
	mTarget.x = 0.0f;
	mTarget.y = BEGIN_TARGET_Y;
	mTarget.z = BattleCharacterManager::ENEMY_POS_Z;
}

void CameraBattle::Update(const Character* target)
{
	BattleState::State state = BattleState::GetInstance().GetState();

	auto CommandSelect = [&]()
	{
		const float COMMAND_SELECT_DIST = 6.0f;
		Vector3 pos = target->GetPos();

		mPos.x = pos.x + sinf(mAngle.y) * COMMAND_SELECT_DIST;
		mPos.y = pos.y + (target->GetAABB().max.y - target->GetAABB().min.y) * 0.5f;
		mPos.z = pos.z + cosf(mAngle.y) * COMMAND_SELECT_DIST;

		constexpr float addAngle = DirectX::XMConvertToRadians(0.1f);
		mAngle.y += addAngle;
		
		mTarget = Vector3(pos.x, mPos.y, pos.z);
	};


	switch (state)
	{
	case BattleState::State::BEGIN:
		mPos = Vector3::Lerp(mBeginPos, mEndPos, mLerpFactor);

		if (mLerpFactor < 1.0f)
		{
			const float LERP_ACCEL = 0.001f;
			mLerpVelocity += LERP_ACCEL;
			mLerpFactor += mLerpVelocity;
		}
		else
		{
			mTimer += GameManager::elapsedTime;
			if (mTimer >= BattleState::BEGIN_TIME)
			{
				BattleState::GetInstance().SetState(BattleState::State::BEGIN_FINISHED);
			}
		}

		break;

	case BattleState::State::COMMAND_SELECT:
		CommandSelect();
		break;

	case BattleState::State::ENEMY_SELECT:
	case BattleState::State::PLAYER_ATTACK:
	case BattleState::State::ENEMY_ATTACK:
	{
		const float POS_Y_DIST = 7.5f;
		const float POS_Z_DIST = 7.5f;
		mPos = Vector3(target->GetPos().x, POS_Y_DIST, BattleCharacterManager::PLAYER_POS_Z - POS_Z_DIST);
		
		float targetX = (BattleCharacterManager::POS_MIN_X + BattleCharacterManager::POS_MAX_X) / 2.0f;
		float targetZ = BattleCharacterManager::ENEMY_POS_Z;
		mTarget = Vector3(targetX, 0.0f, targetZ);

	}
		break;
	case BattleState::State::PARTY_SELECT:
	{
		const float POS_Y_DIST = 8.0f;
		mPos = Vector3(0.0f, POS_Y_DIST, BattleCharacterManager::ENEMY_POS_Z);

		float targetX = (BattleCharacterManager::POS_MIN_X + BattleCharacterManager::POS_MAX_X) / 2.0f;
		float targetY = (target->GetLocalAABB().max.y - target->GetLocalAABB().min.y) / 2.0f;
		float targetZ = BattleCharacterManager::PLAYER_POS_Z;
		mTarget = Vector3(targetX, targetY, targetZ);
	}
		break;
	}
	UpdateView();
}