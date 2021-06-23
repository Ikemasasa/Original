#include "CommandPlayer.h"

#include "BattleActorManager.h"
#include "BattleState.h"
#include "CameraManager.h"
#include "CommandBehaviour.h"
#include "CommandItem.h"
#include "FrontendBattle.h"

#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

CommandPlayer::CommandPlayer() : CommandBase()
{
	mCommand.push(std::make_unique<CommandBehaviour>());
}

CommandPlayer::~CommandPlayer()
{
	size_t size = mCommand.size();
	for (size_t i = 0; i < size; ++i) mCommand.pop();
}

void CommandPlayer::Update(const BattleActorManager* bam)
{
	if (IsBehaviourEnable()) return;

	// コマンドを1つ戻す
	if (mCommand.top()->IsBackState())
	{
		mCommand.pop();
	}

	// 次のコマンドがあるなら更新する
	IDecideCommand* nextCommand = mCommand.top()->ReleaseNextCommandState();
	if (nextCommand)
	{
		mCommand.emplace(nextCommand);
	}

	mCommand.top()->Update(bam, this);

	//BattleState::GetInstance().SetState(BattleState::State::ENEMY_SELECT);
	//std::vector<int> objectIDs = bam->GetObjectIDs(Actor::Type::ENEMY);

	//// objectID選択
	//if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  mTargetObjectIndex = Math::Max(0				     , mTargetObjectIndex - 1);
	//if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) mTargetObjectIndex = Math::Min(objectIDs.size() - 1, mTargetObjectIndex + 1);
	//mTargetObjID = objectIDs[mTargetObjectIndex];

	//// アイコンの座標設定
	//{
	//	const std::shared_ptr<BattleActor> ba = bam->GetActor(mTargetObjID);
	//	constexpr float adjustY = 0.5f;
	//	Vector3 world(ba->GetPos().x, ba->GetAABB().max.y, ba->GetPos().z);

	//	DirectX::XMFLOAT4X4 view = Singleton<CameraManager>().GetInstance().GetView();
	//	DirectX::XMFLOAT4X4 proj = Singleton<CameraManager>().GetInstance().GetProj();
	//	Vector2 p = world.WorldToScreen(view, proj);

	//	Vector2 scale(ICON_SCALE_X, ICON_SCALE_Y);
	//	Vector2 texP(ICON_CHARA_X, 0.0f);
	//	Vector2 size(ICON_SIZE_X, ICON_SIZE_Y);
	//	Vector2 center(ICON_SIZE_X / 2.0f, ICON_SIZE_Y);
	//	FrontendBattle::GetInstance().SetSprite(mIcons, p, scale, texP, size, center);
	//}


	//// 決定
	//if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	//{
	//	mBehaviour = Behaviour::ATTACK; // kari
	//	mCmdState = CommandState::INIT;
	//}
}

void CommandPlayer::Render() const
{
	mCommand.top()->Render();
}

void CommandPlayer::ResetCommand()
{
	size_t popNum = mCommand.size() - 1; // CommandBehaviourだけ残したいから size - 1
	for (size_t i = 0; i < popNum; ++i) mCommand.pop();
}
