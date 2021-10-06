#include "MenuItem.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "Item.h"
#include "ItemData.h"
#include "KeyGuide.h"
#include "Player.h"
#include "PlayerManager.h"
#include "StatusData.h"

void MenuItem::Initialize(const PlayerManager* plm)
{
	mCharacterHealth.Initialize(Vector2(HEALTH_BOARD_X, HEALTH_BOARD_Y));
	mCharacterSelect.Initialize(plm);
	mItemSelect.Initialize();
	mSelectOptions.Initialize();
	mItemIndex = -1;
}

MenuBase::Select MenuItem::Update(PlayerManager* plm)
{
	if (mItemIndex == -1)
	{
		mCharacterSelect.Update(); // �N�̃C���x���g�����Q�Ƃ��邩���߂�

		mInventory = plm->GetPlayer(mCharacterSelect.GetIndex())->GetInventory(); // �Q�Ƃ���C���x���g���ۑ�
		mItemSelect.Update(mInventory); 

		// �A�C�e���I��
		if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		{
			int index = mItemSelect.GetIndex();
			const ItemData::BaseData* base = mInventory->GetItemParam(index);
			
			// �񕜃A�C�e�����A�t�B�[���h�g�p�n�A�C�e���Ȃ�g����
			if (base->type == ItemData::HEAL || base->type == ItemData::FIELD_USE)
			{
				mItemIndex = mItemSelect.GetIndex();
			}
			else
			{
				Audio::SoundStop((int)Sound::SELECT);
				Audio::SoundPlay((int)Sound::CANCEL);
			}
		}

		// �O�̉�ʂɖ߂�
		if (Input::GetButtonTrigger(0, Input::BUTTON::B)) return Select::BACK;

		KeyGuide::Instance().Add(KeyGuide::A, L"����");
		KeyGuide::Instance().Add(KeyGuide::B, L"�߂�");
	}
	else
	{
		// �A�C�e�������擾
		const ItemData::BaseData* baseData = mInventory->GetItemParam(mItemIndex);

		switch (baseData->type)
		{
		case ItemData::HEAL:	  UseHealItem(plm, baseData->id); break;
		case ItemData::FIELD_USE: UseFieldUseItem(baseData->id); break;
		}

		if (Input::GetButtonTrigger(0, Input::BUTTON::B) && !mIsUseFieldItem)
		{
			mItemIndex = -1; // �A�C�e�����I����Ԃɖ߂�
		}

		// �L�[�K�C�h
		KeyGuide::Instance().Add(KeyGuide::B, L"�߂�");
	}

	return MenuBase::NONE;
}

void MenuItem::Render()
{
	mCharacterSelect.Render(Vector2(BOARD_OFFSET_X + ItemSelect::ICON_OFFSET, BOARD_OFFSET_Y));
	mItemSelect.Render(Vector2(BOARD_OFFSET_X, BOARD_OFFSET_Y));

	if (mItemIndex != -1)
	{
		// �A�C�e�����擾
		const ItemData::BaseData* baseData = mInventory->GetItemParam(mItemIndex);

		if (baseData->type == ItemData::HEAL)	   mCharacterHealth.Render(true);
		if (baseData->type == ItemData::FIELD_USE)
		{
			mSelectOptions.Render(Vector2(BOARD_OFFSET_X, BOARD_OFFSET_Y) + mItemSelect.GetCursorRightUpPos());
		}
	}
} 

void MenuItem::Release()
{

}

void MenuItem::UseFieldUseItem(const int itemID)
{
	// �I�����쐬
	mSelectOptions.AddTitle(L"�g�p���܂����H");
	mSelectOptions.AddOption(L"�͂�");
	mSelectOptions.AddOption(L"������");
	mSelectOptions.Update();

	// ���łɎg�p���Ȃ�
	if (mIsUseFieldItem)
	{
		// �A�C�e�������擾
		const ItemData::BaseData* baseData = mInventory->GetItemParam(mItemIndex);
		const FieldUseItemData::Param param = FieldUseItemData::GetParam(baseData->id);
		param.effect->Execute();

		if (param.effect->IsEffectFinished())
		{
			// �A�C�e�����炷
			mInventory->Sub(baseData->id);

			// �A�C�e�����I����Ԃɖ߂�
			mItemIndex = -1;
		}

		return;
	}

	// �I��
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		switch (mSelectOptions.GetIndex())
		{
		case 0: mIsUseFieldItem = true; break;
		case 1: mItemIndex = -1; break;
		}
	}
}

void MenuItem::UseHealItem(const PlayerManager* plm, const int itemID)
{
	// �X�e�[�^�X�z��쐬
	int statusNum = plm->GetNum();
	std::vector<Status> statusArray;
	for (int i = 0; i < statusNum; ++i)
	{
		int charaID = plm->GetPlayer(i)->GetCharaID();
		statusArray.push_back(StatusData::GetPLStatus(charaID));
	}
	mCharacterHealth.Update(statusArray);

	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		// �񕜃A�C�e���̏����擾
		UseItemData::Param param = UseItemData::GetParam(itemID);

		// �X�e�[�^�X�擾
		int charaindex = mCharacterHealth.GetSelectIndex();
		int charaID = plm->GetPlayer(charaindex)->GetCharaID();
		Status plStatus = StatusData::GetPLStatus(charaID);

		// �񕜃A�C�e�����g���邩�`�F�b�N����
		bool useable = false;
		if (!plStatus.IsFullHP() && param.hpValue > 0) useable = true; // HP��max����Ȃ�����hp��
		if (!plStatus.IsFullMP() && param.mpValue > 0) useable = true; // MP��max����Ȃ�����mp��
		if (!useable) // �g���Ȃ��Ȃ�return
		{
			Audio::SoundStop((int)Sound::SELECT);
			Audio::SoundPlay((int)Sound::CANCEL);
			return;
		}

		// ��
		plStatus.AddHP(param.hpValue);
		plStatus.AddMP(param.mpValue);
		Audio::SoundPlay((int)Sound::HEAL);

		// �X�e�[�^�X�X�V
		StatusData::SetPLStatus(charaID, plStatus);

		// �A�C�e�����炷
		mInventory->Sub(param.base->id);

		// �A�C�e�����I����Ԃɖ߂�
		mItemIndex = -1;
	}
}
