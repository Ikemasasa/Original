#include "DecideTargetChara.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CameraManager.h"
#include "CommandBase.h"
#include "Item.h"
#include "Singleton.h"



DecideTargetChara::DecideTargetChara(Character::Type characterType)
{
	mCharaType = characterType;
}

void DecideTargetChara::Initialize(const BattleCharacterManager* bcm)
{
	mArrow = std::make_unique<Sprite>(L"Data/Image/Battle/arrow.png");
	mCharacterHealth.Initialize(Vector2::ZERO); // ���Ń[����n���Ă�
}

void DecideTargetChara::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	// ���ʉ���炷
	SoundPlay();

	// �L�����^�C�v�ɂ����State��ς���
	if (mCharaType == Character::PLAYER)     BattleState::GetInstance().SetState(BattleState::State::PARTY_SELECT);
	else if (mCharaType == Character::ENEMY) BattleState::GetInstance().SetState(BattleState::State::ENEMY_SELECT);

	// �����Ă�L��������I������
	const std::vector<int>& ids = bcm->GetAliveObjIDs(mCharaType);
	int num = static_cast<int>(ids.size());
	if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) mSelectIndex = (mSelectIndex + num - 1) % num; 
	if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  mSelectIndex = (mSelectIndex + 1) % num;
	mTargetChara = bcm->GetChara(ids[mSelectIndex]); // Render�ł������� �����ő��

	// CharacterHealth�X�V
	std::vector<Status> status;
	status.push_back(*mTargetChara->GetStatus());
	mCharacterHealth.Update(status);
	

	// �L�����I��������
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		cmdBase->AddTargetObjID(ids[mSelectIndex]);
		
		// �X�L���A�A�C�e�����o�^����Ă��邩�`�F�b�N
		if		(cmdBase->IsUseItem()) SetBehaviourUseItem(cmdBase);
		else if (cmdBase->IsUseSkill())   SetBehaviourUseSkill(cmdBase);
		else							  SetBehaviourAttack(cmdBase);
	}

	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
	}
}

void DecideTargetChara::Render()
{
	const CameraManager& camera = Singleton<CameraManager>().GetInstance();

	Vector3 targetPos = mTargetChara->GetPos();
	targetPos.y += mTargetChara->GetLocalAABB().max.y;
	Vector2 screen = targetPos.WorldToScreen(camera.GetView(), camera.GetProj());

	Vector2 scale(ARROW_SCALE, ARROW_SCALE);
	Vector2 texPos(0.0f, 0.0f);
	Vector2 size(mArrow->GetSize());
	Vector2 center(size.x / 2.0f, size.y);
	mArrow->Render(screen, scale, texPos, size, center);

	// CharacterHealth�̃{�[�h�̈ʒu�X�V
	Vector2 boardSize = mCharacterHealth.GetBoardSize();
	Vector2 boardPos(screen.x - (boardSize.x / 2.0f), screen.y - boardSize.y - (size.y * scale.y));
	mCharacterHealth.SetLeftTopPos(boardPos);
	mCharacterHealth.Render();
}

void DecideTargetChara::SetBehaviourAttack(CommandBase* cmdBase)
{
	cmdBase->SetBehaviour(CommandBase::Behaviour::ATTACK);
}

void DecideTargetChara::SetBehaviourUseItem(CommandBase* cmdBase)
{
	// �g�p�A�C�e�����񕜃A�C�e���Ȃ� HP�AMP���ő傩�`�F�b�N
	bool isUseable = true;
	const UseItemData::Param* itemParam = cmdBase->GetItemParam();
	if (itemParam->base->type == ItemData::HEAL)
	{
		//HP�񕜂�HP���}�b�N�X, MP�񕜂�MP���}�b�N�X �Ȃ�g�p�s��
		if (itemParam->hpValue > 0 && mTargetChara->GetStatus()->IsFullHP()) isUseable = false;
		if (itemParam->mpValue > 0 && mTargetChara->GetStatus()->IsFullMP()) isUseable = false;
	}

	if (isUseable) cmdBase->SetBehaviour(CommandBase::Behaviour::USE_ITEM);
	else Audio::SoundPlay((int)Sound::CANCEL);// �g���Ȃ��Ȃ�L�����Z����
}

void DecideTargetChara::SetBehaviourUseSkill(CommandBase* cmdBase)
{
	cmdBase->SetBehaviour(CommandBase::Behaviour::SKILL);
}

