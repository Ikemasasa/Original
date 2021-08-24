#include "MenuItem.h"

#include <map>

#include "lib/Audio.h"
#include "lib/ConvertString.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "Item.h"
#include "ItemData.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Singleton.h"
#include "StatusData.h"

void MenuItem::Initialize(const PlayerManager* plm)
{
	mCharacterHealth.Initialize(Vector2(HEALTH_BOARD_X, HEALTH_BOARD_Y));
	mCharacterSelect.Initialize(plm);
	mItemSelect.Initialize();
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
			const ItemData::ItemParam* param = mInventory->GetItemParam(index);
			
			// �񕜃A�C�e���Ȃ�g����
			if(param->effect == ItemData::Effect::HEAL)	mItemIndex = mItemSelect.GetIndex();
			else
			{
				AUDIO.SoundStop((int)Sound::SELECT);
				AUDIO.SoundPlay((int)Sound::CANCEL);
			}
		}

		// �O�̉�ʂɖ߂�
		if (Input::GetButtonTrigger(0, Input::BUTTON::B))
			return Select::BACK;
	}
	else 
	{
		// �X�e�[�^�X�z��쐬
		int statusNum = plm->GetNum();
		std::vector<Status> statusArray;
		for (int i = 0; i < statusNum; ++i)
		{
			int charaID = plm->GetPlayer(i)->GetCharaID();
			statusArray.push_back(Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(charaID));
		}
		mCharacterHealth.Update(statusArray);

		if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		{
			// ����̃T�E���h��炳�Ȃ�
			AUDIO.SoundStop((int)Sound::SELECT);

			int charaindex = mCharacterHealth.GetSelectIndex();
			int id = plm->GetPlayer(charaindex)->GetCharaID();

			// �X�e�[�^�X�X�V
			const ItemData::ItemParam* param = mInventory->GetItemParam(mItemIndex);
			Status plStatus = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(id);

			bool isHeal = false;
			if (!plStatus.IsFullHP() && param->hpValue > 0) isHeal = true; // HP��max����Ȃ�����hp��
			if (!plStatus.IsFullMP() && param->mpValue > 0) isHeal = true; // MP��max����Ȃ�����mp��
			if (isHeal) // ��
			{
				plStatus.AddHP(param->hpValue);
				plStatus.AddMP(param->mpValue);
				Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(id, plStatus); //�X�e�[�^�X�X�V
				mInventory->Sub(param->id); // �A�C�e�����炷

				AUDIO.SoundPlay((int)Sound::HEAL);
				mItemIndex = -1;// �A�C�e�����I����Ԃɖ߂�
			}
			else // �񕜂��Ȃ�
			{
				AUDIO.SoundPlay((int)Sound::CANCEL);
			}
		}

		else if(Input::GetButtonTrigger(0, Input::BUTTON::B))
		{
			mItemIndex = -1; // �A�C�e�����I����Ԃɖ߂�
		}
	}

	return MenuBase::NONE;
}

void MenuItem::Render()
{
	mCharacterSelect.Render(Vector2(BOARD_OFFSET_X + ItemSelect::ICON_OFFSET, BOARD_OFFSET_Y));
	mItemSelect.Render(Vector2(BOARD_OFFSET_X, BOARD_OFFSET_Y));

	if (mItemIndex != -1)
	{
		mCharacterHealth.Render(true);
	}
} 

void MenuItem::Release()
{

}