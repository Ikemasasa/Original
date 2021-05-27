#include "CameraBattle.h"

#include "BattleActorManager.h"
#include "BattleState.h"
#include "Define.h"

CameraBattle::CameraBattle() : CameraBase()
{

}

void CameraBattle::Update(const std::shared_ptr<Actor>& target)
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
		mPos = Vector3(0.0f, 7.5f, BattleActorManager::PLAYER_POS_Z - 7.5f);
		mTarget = Vector3(0.0f, 0.0f, BattleActorManager::ENEMY_POS_Z);
		break;

	case BattleState::State::ATTACK:
		Vector3 p = target->GetPos(); // 座標
		Vector3 t = target->GetPos(); // ターゲット

		// 左下について欲しい
		{
			constexpr float ADD = DirectX::XMConvertToRadians(-100.0f); // -100度
			float x = sinf(target->GetAngle().y + ADD);
			float y = (target->GetAABB().max.y - target->GetAABB().min.y) * 0.5f;
			float z = cosf(target->GetAngle().y + ADD);
			Vector3 leftDown(x, y, z);
			leftDown.Normalize();

			const float DIST = 30;
			p.x += leftDown.x * DIST;
			p.y = leftDown.y;
			p.z += leftDown.z * DIST;

			Vector3 front(sinf(target->GetAngle().y), 0.0f, cosf(target->GetAngle().y));

			t.x += front.x;
			t.y += y;
			t.z += front.z;
		}
		mPos = p;
		mTarget = t;
		break;
	}

	UpdateView();
}