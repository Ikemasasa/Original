#include "CameraBattle.h"

#include "BattleActorManager.h"
#include "BattleState.h"
#include "Define.h"

CameraBattle::CameraBattle() : CameraBase()
{

}

void CameraBattle::Update(const Actor* target)
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
		
		const float TARGET_DIST = 20.0f;
		// 動くアクターの上から
		{
			Vector3 nowDist(sinf(target->GetAngle().y), 0.0f, cosf(target->GetAngle().y));

			// target
			t += nowDist * TARGET_DIST;

			// POS
			// Y
			const AABB& targetAABB = target->GetLocalAABB();
			p.y += targetAABB.max.y * 1.5f; // モデル1.5体分上に

			// X Z
			const float POS_DIST = 10.0f;
			p += -(nowDist * POS_DIST);
		}
		mPos = p;
		mTarget = t;
		break;
	}

	UpdateView();
}