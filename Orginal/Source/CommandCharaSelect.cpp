#include "CommandCharaSelect.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"

#include "BattleCharacterManager.h"
#include "BattleState.h"
#include "CameraManager.h"
#include "CommandBase.h"
#include "Item.h"
#include "Singleton.h"

CommandCharaSelect::CommandCharaSelect(Character::Type characterType)
{
	mArrow = std::make_unique<Sprite>(L"Data/Image/Battle/arrow.png");
	mCharaType = characterType;
}

void CommandCharaSelect::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	// �L�����^�C�v�ɂ����State��ς���
	if (mCharaType == Character::PLAYER)     BattleState::GetInstance().SetState(BattleState::State::PARTY_SELECT);
	else if (mCharaType == Character::ENEMY) BattleState::GetInstance().SetState(BattleState::State::ENEMY_SELECT);

	// �����Ă铯���^�C�v�̃A�N�^�[����I������
	const std::vector<int>& ids = bcm->GetAliveCharaIDs(mCharaType);
	int max = static_cast<int>(ids.size() - 1);
	if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) mCharaIndex = Math::Min(mCharaIndex + 1, max);
	if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  mCharaIndex = Math::Max(mCharaIndex - 1, 0);
	mTargetChara = bcm->GetChara(ids[mCharaIndex]); // Render�ł������� �����ő��

	// �L�����I��������
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		cmdBase->SetTargetObjID(ids[mCharaIndex]);
		
		// �A�C�e�����o�^����Ă��邩�ǂ����ŕ���
		if (cmdBase->GetItemIndex() != -1)
		{
			// �A�C�e���g�p�҂̃C���x���g���擾
			BattleCharacter* moveChara = bcm->GetMoveChara();
			Item* moveCharaInventory = moveChara->GetInventory();
			
			// �g�p�A�C�e�����񕜃A�C�e���Ȃ� HP�AMP���ő傩�`�F�b�N
			bool isUseable = true;
			const ItemData::ItemParam* itemParam = moveCharaInventory->GetItemParam(cmdBase->GetItemIndex());
			if (itemParam->effect == ItemData::HEAL)
			{
				//HP�񕜂�HP���}�b�N�X, MP�񕜂�MP���}�b�N�X �Ȃ�g�p�s��
				if      (itemParam->hpValue > 0 && moveChara->GetStatus()->IsFullHP()) isUseable = false;
				else if (itemParam->mpValue > 0 && moveChara->GetStatus()->IsFullMP()) isUseable = false;
			}

			if (isUseable) cmdBase->SetBehaviour(CommandBase::Behaviour::USE_ITEM);
			else AUDIO.SoundPlay((int)Sound::CANCEL);// �g���Ȃ��Ȃ�L�����Z����
		}
		else // �A�C�e�����o�^����Ă��Ȃ�
		{
			cmdBase->SetBehaviour(CommandBase::Behaviour::ATTACK);
		}
	}

	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		mIsBackState = true;
	}
}

void CommandCharaSelect::Render()
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
}