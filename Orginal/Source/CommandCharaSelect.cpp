#include "CommandCharaSelect.h"

#include "lib/Input.h"
#include "lib/Math.h"

#include "BattleActorManager.h"
#include "BattleState.h"
#include "CameraManager.h"
#include "CommandBase.h"
#include "Singleton.h"

CommandCharaSelect::CommandCharaSelect(Actor::Type characterType)
{
	mArrow = std::make_unique<Sprite>(L"Data/Image/Battle/arrow.png");
	mCharaType = characterType;
}

void CommandCharaSelect::Update(const BattleActorManager* bam, CommandBase* cmdBase)
{
	// �L�����^�C�v�ɂ����State��ς���
	if (mCharaType == Actor::PLAYER)     BattleState::GetInstance().SetState(BattleState::State::PARTY_SELECT);
	else if (mCharaType == Actor::ENEMY) BattleState::GetInstance().SetState(BattleState::State::ENEMY_SELECT);

	// �����Ă铯���^�C�v�̃A�N�^�[����I������
	const std::vector<int>& ids = bam->GetAliveActorIDs(mCharaType);
	int max = static_cast<int>(ids.size() - 1);
	if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) mCharaIndex = Math::Min(mCharaIndex + 1, max);
	if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  mCharaIndex = Math::Max(mCharaIndex - 1, 0);
	mTargetActor = bam->GetActor(ids[mCharaIndex]);

	// �L�����I��������
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		cmdBase->SetTargetObjID(ids[mCharaIndex]);

		// �A�C�e�����o�^����Ă��邩�ǂ����ŕ���
		if (cmdBase->GetItemParam()) cmdBase->SetBehaviour(CommandBase::Behaviour::USE_ITEM);
		else						 cmdBase->SetBehaviour(CommandBase::Behaviour::ATTACK);
	}

	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
	}
}

void CommandCharaSelect::Render() const
{
	const CameraManager& camera = Singleton<CameraManager>().GetInstance();

	Vector3 targetPos = mTargetActor->GetPos() + Vector3(0, mTargetActor->GetLocalAABB().max.y, 0);
	Vector2 screen = targetPos.WorldToScreen(camera.GetView(), camera.GetProj());

	Vector2 scale(ARROW_SCALE, ARROW_SCALE);
	Vector2 texPos(0.0f, 0.0f);
	Vector2 size(mArrow->GetSize());
	Vector2 center(size.x / 2.0f, size.y);
	mArrow->Render(screen, scale, texPos, size, center);
}