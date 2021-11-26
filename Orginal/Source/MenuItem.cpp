#include "MenuItem.h"

#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "Item.h"
#include "ItemData.h"
#include "KeyGuide.h"
#include "Player.h"
#include "PlayerManager.h"
#include "StatusData.h"
#include "Sound.h"

void MenuItem::Initialize(const PlayerManager* plm)
{
	// �w���X�Q�[�W�쐬
	mHealthGauge.resize(plm->GetNum());
	for (auto& gauge : mHealthGauge) gauge.Initialize();

	// �L�����N�^�[�I���N���X������
	mCharacterSelect.Initialize(plm);

	// �A�C�e���I���N���X������
	mItemSelect.Initialize();

	// �I�����I���N���X������
	mSelectOptions.Initialize();

	// �A�C�e���𖢑I����Ԃɐݒ�
	mItemIndex = -1;
}

MenuBase::Select MenuItem::Update(PlayerManager* plm)
{
	// �A�C�e�����I����ԂȂ�
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
				Sound::Stop(Sound::SELECT);
				Sound::Play(Sound::CANCEL);
			}
		}

		// �O�̉�ʂɖ߂�
		if (Input::GetButtonTrigger(0, Input::BUTTON::B)) return Select::BACK;

		KeyGuide::Instance().Add(KeyGuide::A, L"����");
		KeyGuide::Instance().Add(KeyGuide::B, L"�߂�");
	}
	
	// �A�C�e���I����ԂȂ�
	if (mItemIndex != -1)
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
		UseItemData::Param param = UseItemData::GetParam(baseData->id);

		if (baseData->type == ItemData::HEAL)
		{
			for (size_t i = 0; i < mHealthGauge.size(); ++i)
			{
				bool isSelectRender = false;

				// �A�C�e���͈̔͂��S��, �������͑I�𒆂̃L�����N�^�[�̃Q�[�W�Ȃ�I���摜��\��
				if (param.range == UseItemData::Range::ALL ||
					i == mCharacterSelect.GetIndex())
				{
					isSelectRender = true;
				}

				// �Q�[�W��`��
				mHealthGauge[i].Render(isSelectRender);


			}
		}
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

	if (mIsUseFieldItem)
	{
		// �A�C�e�������擾
		const ItemData::BaseData* baseData = mInventory->GetItemParam(mItemIndex);
		const FieldUseItemData::Param param = FieldUseItemData::GetParam(baseData->id);
		param.effect->Execute();

		// �A�C�e���̏������I��������
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
	// HealthGauge�X�e�[�^�X�ݒ�
	for (size_t i = 0; i < plm->GetNum(); ++i)
	{
		// �L�����N�^ID���擾
		int charaID = plm->GetPlayer(i)->GetCharaID();

		// ���W�Z�o
		float x = HEALTH_BOARD_X;
		float y = HEALTH_BOARD_Y + mHealthGauge[i].GetHeight() * i;
		Vector2 pos(x, y);

		// �ݒ�
		mHealthGauge[i].Set(StatusData::GetPLStatus(charaID), pos, HealthGauge::LEFTTOP);
	}

	// �񕜃A�C�e���̏����擾
	UseItemData::Param param = UseItemData::GetParam(itemID);

	// �Ώۂ̃L�����N�^�[�I��
	if (param.range == UseItemData::SINGLE)
	{
		size_t max = plm->GetNum();
		if (Input::GetButtonTrigger(0, Input::UP))   mSelectIndex = (mSelectIndex + (max - 1)) % max;
		if (Input::GetButtonTrigger(0, Input::DOWN)) mSelectIndex = (mSelectIndex + 1) % max;
	}
	if (param.range == UseItemData::ALL)
	{
		// �S���I���Ȃ�-1
		mSelectIndex = -1;
	}


	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		// �X�e�[�^�X�擾
		std::vector<Status> targetStatus;

		// mSelectIndex�� -1 �Ȃ�S���I��
		if (mSelectIndex == -1)
		{
			for (size_t i = 0; i < plm->GetNum(); ++i)
			{
				Status status = StatusData::GetPLStatus(plm->GetPlayer(i)->GetCharaID());
				targetStatus.emplace_back(status);
			}
		}
		else
		{ 
			// mSelectIndex��-1����Ȃ��Ȃ�P��
			Status status = StatusData::GetPLStatus(plm->GetPlayer(mSelectIndex)->GetCharaID());
			targetStatus.emplace_back(status);
		}

		// �񕜃A�C�e�����g���邩�`�F�b�N����
		bool useable = false;
		for (const auto& status : targetStatus)
		{
			if (!status.IsFullHP() && param.hpValue > 0) useable = true; // HP��max����Ȃ�����hpv��
			if (!status.IsFullMP() && param.mpValue > 0) useable = true; // MP��max����Ȃ�����mp��
			if (useable) break; // �N��1�l�ł��g������g����
		}

		if (!useable) // �g���Ȃ��Ȃ�return
		{
			Audio::SoundStop((int)Sound::SELECT);
			Audio::SoundPlay((int)Sound::CANCEL);
			return;
		}

		// ��
		for (auto& status : targetStatus)
		{
			status.AddHP(param.hpValue);
			status.AddMP(param.mpValue);

			// �X�e�[�^�X�X�V
			StatusData::SetPLStatus(status.GetID(), status);
		}
		Audio::SoundPlay((int)Sound::HEAL);

		// �A�C�e�����炷
		mInventory->Sub(param.base->id);

		// �A�C�e�����I����Ԃɖ߂�
		mItemIndex = -1;
	}
}
