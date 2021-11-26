#include "DecideItem.h"

#include "lib/Input.h"

#include "BattleCharacterManager.h"
#include "CommandBase.h"
#include "DecideTargetChara.h"
#include "Define.h"
#include "Item.h"
#include "KeyGuide.h"

void DecideItem::Initialize(const BattleCharacterManager* bcm)
{
	mItemSelect.Initialize();
}

void DecideItem::Update(const BattleCharacterManager* bcm, CommandBase* cmdBase)
{
	// ���ʉ���炷
	SoundPlay();

	BattleCharacter* moveChara = bcm->GetMoveChara();
	mItemSelect.Update(moveChara->GetInventory());

	// �A�C�e���I��������
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		// �A�C�e����{�����擾
		const ItemData::BaseData* base = moveChara->GetInventory()->GetItemParam(mItemSelect.GetIndex());

		// �񕜁A�_���[�W�A�C�e���Ȃ�
		if (base->type == ItemData::HEAL || base->type == ItemData::DAMAGE)
		{
			// �g�p�A�C�e�������擾
			UseItemData::Param param = UseItemData::GetParam(base->id);
			cmdBase->SetItemParam(&param);

			// ���̃R�}���h�����߂�
			Character::Type charaType = Character::NONE;
			switch (param.target)
			{
			case UseItemData::Target::PARTY: charaType = Character::PLAYER; break;
			case UseItemData::Target::ENEMY: charaType = Character::ENEMY;  break;
			}
			mNextCommand = std::make_unique<DecideTargetChara>(charaType);
		}
	}

	// CommandBehaviour�ɖ߂�
	if (Input::GetButtonTrigger(0, Input::BUTTON::B))
	{
		cmdBase->SetItemParam(nullptr);
		mIsBackState = true;
	}

	// �L�[�K�C�h�ݒ�
	KeyGuide::Instance().Add(KeyGuide::A, L"����");
	KeyGuide::Instance().Add(KeyGuide::B, L"�߂�");
}

void DecideItem::Render()
{
	const Vector2 boardOffset(Define::SCREEN_WIDTH / 10.0f, Define::SCREEN_HEIGHT / 5.0f);
	mItemSelect.Render(boardOffset);
}
