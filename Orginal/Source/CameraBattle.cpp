#include "CameraBattle.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "Character.h"
#include "Define.h"


CameraBattle::CameraBattle() : CameraBase()
{

}

void CameraBattle::Update(const Character* target)
{
	BattleState::State state = BattleState::GetInstance().GetState();

	auto SortOrder = [&]()
	{
		const float SORT_ORDER_DIST = 30.0f;

		mPos.x = sinf(mAngle.y) * SORT_ORDER_DIST;
		mPos.y = 20.0f;
		mPos.z = cosf(mAngle.y) * SORT_ORDER_DIST;

		constexpr float addAngle = DirectX::XMConvertToRadians(0.5f);
		mAngle.y += addAngle;
		mTarget = Vector3(0.0f, 0.0f, 0.0f);
	};

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
	case BattleState::State::SORT_ORDER:
		SortOrder();
		break;

	case BattleState::State::COMMAND_SELECT:
		CommandSelect();
		break;

	case BattleState::State::ENEMY_SELECT:
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