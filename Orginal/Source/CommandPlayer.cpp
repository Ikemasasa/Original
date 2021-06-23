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

	// �R�}���h��1�߂�
	if (mCommand.top()->IsBackState())
	{
		mCommand.pop();
	}

	// ���̃R�}���h������Ȃ�X�V����
	IDecideCommand* nextCommand = mCommand.top()->ReleaseNextCommandState();
	if (nextCommand)
	{
		mCommand.emplace(nextCommand);
	}

	mCommand.top()->Update(bam, this);

	//BattleState::GetInstance().SetState(BattleState::State::ENEMY_SELECT);
	//std::vector<int> objectIDs = bam->GetObjectIDs(Actor::Type::ENEMY);

	//// objectID�I��
	//if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  mTargetObjectIndex = Math::Max(0				     , mTargetObjectIndex - 1);
	//if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) mTargetObjectIndex = Math::Min(objectIDs.size() - 1, mTargetObjectIndex + 1);
	//mTargetObjID = objectIDs[mTargetObjectIndex];

	//// �A�C�R���̍��W�ݒ�
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


	//// ����
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
	size_t popNum = mCommand.size() - 1; // CommandBehaviour�����c���������� size - 1
	for (size_t i = 0; i < popNum; ++i) mCommand.pop();
}
