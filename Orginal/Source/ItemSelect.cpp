#include "ItemSelect.h"

#include "lib/Audio.h"
#include "lib/ConvertString.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "Item.h"


void ItemSelect::Initialize()
{
	mSelectIndex = 0;
	mOldSelectIndex = -1;

	mBoard = std::make_unique<Sprite>(L"Data/Image/board.png");
	mSelectFrame = std::make_unique<Sprite>(L"Data/Image/select_frame.png");
	mInfoBoard = std::make_unique<Sprite>(L"Data/Image/item_info_board.png");
}

int ItemSelect::Update(const Item* inventory)
{
	// SelectIndex����
	{
		mInventory = inventory;
		int inventorySize = mInventory->GetItemNum();

		if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) ++mSelectIndex;
		if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  --mSelectIndex;
		if (mSelectIndex >= inventorySize) mSelectIndex -= inventorySize;
		if (mSelectIndex < 0) mSelectIndex += inventorySize;

		if (Input::GetButtonTrigger(0, Input::BUTTON::UP))    mSelectIndex -= HORIZONTAL_NUM;
		if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN))  mSelectIndex += HORIZONTAL_NUM;
		if (mSelectIndex >= inventorySize) mSelectIndex -= inventorySize + 1;
		if (mSelectIndex < 0) mSelectIndex += inventorySize + 1;
		mSelectIndex = Math::Clamp(mSelectIndex, 0, inventorySize - 1); // ���K����񂩂�������clamp���Ă�

		// �A�C�e�����X�V
		UpdateInfo();

		if (mOldSelectIndex != mSelectIndex) AUDIO.SoundPlay((int)Sound::CURSOR_MOVE);
		mOldSelectIndex = mSelectIndex;
	}



	// �A�C�e���I��
	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		return mSelectIndex;
	}

	return -1;
}

void ItemSelect::Render(const Vector2& boardOffset)
{
	// �{�[�h�`��
	mBoard->Render(boardOffset, Vector2::ONE, Vector2::ZERO, mBoard->GetSize());


	// �A�C�e���A�C�R���`��
	{
		// �I�������C���x���g���̃A�C�e����S���`��
		const float ICON_SCALE_SIZE = ICON_SCALE * ICON_SIZE;
		const Vector2 offset(ICON_OFFSET + boardOffset.x, ICON_OFFSET + boardOffset.y);
		const Vector2 scale(ICON_SCALE, ICON_SCALE);
		const Vector2 size(ICON_SIZE, ICON_SIZE);

		int itemNum = mInventory->GetItemNum();
		for (int i = 0; i < itemNum; ++i)
		{
			float x = i % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x;
			float y = i / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y;
			Vector2 pos(x, y);
			mInventory->GetItemParam(i)->icon->Render(pos, scale, Vector2::ZERO, size);
		}

		// �I���̃t���[���摜��`��
		Vector2 pos(mSelectIndex % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x, mSelectIndex / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y);
		mSelectFrame->Render(pos, scale, Vector2::ZERO, size);
	}

	// �A�C�e�����`��
	{
		const Vector2 texPos(0.0f, 0.0f);

		// �{�[�h
		const Vector2 infoBoardPos(boardOffset.x + INFO_BOARD_OFFSET_X, boardOffset.y);
		const Vector2 scale(1.0f, 1.0f);
		mInfoBoard->Render(infoBoardPos, scale, texPos, mInfoBoard->GetSize());

		// �A�C�e���A�C�R��
		const Vector2 iconPos(infoBoardPos.x + INFO_ICON_OFFSET_X, infoBoardPos.y + INFO_ICON_OFFSET_Y);
		const Vector2 iconScale(INFO_ICON_SCALE, INFO_ICON_SCALE);
		const Vector2 iconSize(ICON_SIZE, ICON_SIZE);
		mInventory->GetItemParam(mSelectIndex)->icon->Render(iconPos, iconScale, texPos, iconSize);

		// ����
		RenderSetInfo(infoBoardPos);
		mFont.Render();
	}

}


void ItemSelect::UpdateInfo()
{
	// �C���t�H�X�V
	{
		// �C���f�b�N�X���X�V���ꂽ��
		if (mOldSelectIndex != mSelectIndex)
		{
			// �O��̃A�C�e�������폜
			mInfo.clear();

			const ItemData::ItemParam* param = mInventory->GetItemParam(mSelectIndex);
			std::string range;

			// ���O
			mInfo.push_back(ConvertString::ConvertToWstirng(param->name));

			// hp value
			if (param->hpValue > 0) mInfo.push_back(L"HP���񕜂���");
			if (param->hpValue < 0) mInfo.push_back(L"�G�Ƀ_���[�W��^����");

			// mpvalue
			if (param->mpValue > 0) mInfo.push_back(L"MP���񕜂���");

			// �o�t�n
			if (param->atkValue > 0) mInfo.push_back(L"�U���͂����߂�");
			if (param->defValue > 0) mInfo.push_back(L"����͂����߂�");

			// �t�H���g�쐬
			for (size_t i = 0; i < mInfo.size(); ++i)
			{
				if (mFont.Find(mInfo[i].c_str())) continue;

				// ���쐬�Ȃ���
				mFont.Add(mInfo[i].c_str());
			}
		}
	}

}

void ItemSelect::RenderSetInfo(const Vector2& infoBoardPos)
{
	Vector2 pos;// ���ƂŃ{�[�h�̍��W�𑫂�(���΍��W
	Vector2 center(0.0f, 0.0f);
	Vector2 scale(1.0f, 1.0f);
	Vector4 color(0.51f, 0.22f, 0.0f, 1.0f); // �K���Ɍ��߂Ă�
	
	const float NAME_OFFSET_Y = 28;
	const float INFO_OFFSET_Y = 256.0f;
	const float INFO_Y_ADD = 46;
	for (size_t i = 0; i < mInfo.size(); ++i)
	{
		// �A�C�e�������A�A�C�e���̏�񂩂ŕ�����
		if (i == 0)
		{
			// ���O
			pos = infoBoardPos + Vector2(mInfoBoard->GetSize().x / 2.0f, NAME_OFFSET_Y);
			center = Vector2(mFont.GetWidth(mInfo[i].c_str()) / 2.0f, 0.0f);
			mFont.RenderSet(mInfo[i].c_str(), pos, center, scale, color);
		}
		else
		{
			// ���
			pos = infoBoardPos + Vector2(mInfoBoard->GetSize().x / 2.0f, INFO_OFFSET_Y + INFO_Y_ADD * (i - 1)); // name�͊܂߂Ȃ�����-1
			center = Vector2(mFont.GetWidth(mInfo[i].c_str()) / 2.0f, 0.0f);
			mFont.RenderSet(mInfo[i].c_str(), pos, center, scale, color);
		}
	}



}
