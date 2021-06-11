#include "CommandPlayer.h"

#include "BattleActorManager.h"
#include "BattleState.h"
#include "CameraManager.h"
#include "FrontendBattle.h"

#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

CommandPlayer::CommandPlayer() : CommandBase()
{
	mIcons = std::make_shared<Sprite>();
	mIcons->Load(L"Data/Image/CommandIcons.png");
}

CommandPlayer::~CommandPlayer()
{
}

void CommandPlayer::Update(const BattleActorManager* bam)
{
	if (IsBehaviourEnable()) return;

	const int COMMAND_MIN_X = 0;
	const int COMMAND_MAX_X = 2;
	const int COMMAND_MIN_Y = 0;
	const int COMMAND_MAX_Y = 2;

	// Mediatorパターン使えばもっと綺麗に作れそう？

	switch (mCmdState)
	{
	case CommandState::INIT:
		mCommandX = 1;
		mCommandY = 1;
		mCmdState = CommandState::SELECT_BEHAVIOUR;
		mBehaviour = Behaviour::NONE;
		// break;

	case CommandState::SELECT_BEHAVIOUR:
		// もっと最適化できると思うけど、とりあえずゴリ
		// R：攻撃 L:防御 (これはあとでいい)
		//    道
		// 防 攻 技
		//    逃

		if (Input::GetButtonTrigger(0, Input::UP))
		{
			if (mCommandX == 0 || mCommandX == 2) mCommandX = 1;
			mCommandY = Math::Max(mCommandY - 1, COMMAND_MIN_Y);
		}
		if (Input::GetButtonTrigger(0, Input::DOWN))
		{
			if (mCommandX == 0 || mCommandX == 2) mCommandX = 1;
			mCommandY = Math::Min(mCommandY + 1, COMMAND_MAX_Y);
		}
		if (Input::GetButtonTrigger(0, Input::RIGHT))
		{
			if (mCommandY == 0 || mCommandY == 2) mCommandY = 1;
			mCommandX = Math::Min(mCommandX + 1, COMMAND_MAX_X);
		}
		if (Input::GetButtonTrigger(0, Input::LEFT))
		{
			if (mCommandY == 0 || mCommandY == 2) mCommandY = 1;
			mCommandX = Math::Max(mCommandX - 1, COMMAND_MIN_X);
		}

		// コマンドスプライト
		{
			const float offsetX = 300.0f;
			const float offsetY = 300.0f;

			const Vector2 scale(ICON_SCALE_X, ICON_SCALE_Y);
			const Vector2 scaleSize = Vector2(ICON_SIZE_X * scale.x, ICON_SIZE_Y * scale.y);

			FrontendBattle::GetInstance().SetSprite(mIcons, Vector2(offsetX + scaleSize.x, offsetY), scale, Vector2(ICON_ITEM_X, 0.0f), Vector2(ICON_SIZE_X, ICON_SIZE_Y));
			FrontendBattle::GetInstance().SetSprite(mIcons, Vector2(offsetX, offsetY + scaleSize.y), scale, Vector2(ICON_DEFENCE_X, 0.0f), Vector2(ICON_SIZE_X, ICON_SIZE_Y));
			FrontendBattle::GetInstance().SetSprite(mIcons, Vector2(offsetX + scaleSize.x, offsetY + scaleSize.y), scale, Vector2(ICON_ATTACK_X, 0.0f), Vector2(ICON_SIZE_X, ICON_SIZE_Y));
			FrontendBattle::GetInstance().SetSprite(mIcons, Vector2(offsetX + scaleSize.x * 2, offsetY + scaleSize.y), scale, Vector2(ICON_SKILL_X, 0.0f), Vector2(ICON_SIZE_X, ICON_SIZE_Y));
			FrontendBattle::GetInstance().SetSprite(mIcons, Vector2(offsetX + scaleSize.x, offsetY + scaleSize.y * 2), scale, Vector2(ICON_ESCAPE_X, 0.0f), Vector2(ICON_SIZE_X, ICON_SIZE_Y));
			FrontendBattle::GetInstance().SetSprite(mIcons, Vector2(offsetX + mCommandX * scaleSize.x, offsetY + mCommandY * scaleSize.y), scale, Vector2(ICON_SELECT_X, 0.0f), Vector2(ICON_SIZE_X, ICON_SIZE_Y));
		}


		if (Input::GetButtonTrigger(0, Input::A))
		{
			if (mCommandY == 0) mCmdState = CommandState::SELECT_ITEM;
			if (mCommandY == 2) mCmdState = CommandState::SELECT_ESCAPE;

			if (mCommandY == 1)
			{
				if (mCommandX == 0) mCmdState = CommandState::SELECT_DEFENCE;
				if (mCommandX == 1) mCmdState = CommandState::SELECT_ATTACK;
				if (mCommandX == 2) mCmdState = CommandState::SELECT_SKILL;
			}
		}
		break;
	case CommandState::SELECT_ITEM:mCmdState = CommandState::SELECT_ENEMY; break;
	case CommandState::SELECT_DEFENCE:mCmdState = CommandState::SELECT_ENEMY; break;

	case CommandState::SELECT_ATTACK:
		mCmdState = CommandState::SELECT_ENEMY;
		break;

	case CommandState::SELECT_SKILL:mCmdState = CommandState::SELECT_ENEMY; break;
	case CommandState::SELECT_ESCAPE:mCmdState = CommandState::SELECT_ENEMY; break;

	case CommandState::SELECT_ENEMY:
		BattleState::GetInstance().SetState(BattleState::State::ENEMY_SELECT);
		std::vector<int> objectIDs = bam->GetObjectIDs(Actor::Type::ENEMY);

		// objectID選択
		if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  mTargetObjectIndex = Math::Max(0				     , mTargetObjectIndex - 1);
		if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) mTargetObjectIndex = Math::Min(objectIDs.size() - 1, mTargetObjectIndex + 1);
		mTargetObjID = objectIDs[mTargetObjectIndex];

		// アイコンの座標設定
		{
			const std::shared_ptr<BattleActor> ba = bam->GetActor(mTargetObjID);
			constexpr float adjustY = 0.5f;
			Vector3 world(ba->GetPos().x, ba->GetAABB().max.y, ba->GetPos().z);

			DirectX::XMFLOAT4X4 view = Singleton<CameraManager>().GetInstance().GetView();
			DirectX::XMFLOAT4X4 proj = Singleton<CameraManager>().GetInstance().GetProj();
			Vector2 p = world.WorldToScreen(view, proj);

			Vector2 scale(ICON_SCALE_X, ICON_SCALE_Y);
			Vector2 texP(ICON_CHARA_X, 0.0f);
			Vector2 size(ICON_SIZE_X, ICON_SIZE_Y);
			Vector2 center(ICON_SIZE_X / 2.0f, ICON_SIZE_Y);
			FrontendBattle::GetInstance().SetSprite(mIcons, p, scale, texP, size, center);
		}


		// 決定
		if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		{
			mBehaviour = Behaviour::ATTACK; // kari
			mCmdState = CommandState::INIT;
		}
		break;
	}
}
