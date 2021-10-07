#include "ItemSelect.h"

#include "lib/Audio.h"
#include "lib/ConvertString.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "Define.h"
#include "KeyGuide.h"
#include "Item.h"

void ItemSelect::Initialize()
{
	mSelectIndex = 0;

	mItemBoard = std::make_unique<Sprite>(L"Data/Image/Menu/item_board.png");
	mSelectFrame = std::make_unique<Sprite>(L"Data/Image/Menu/item_board_select.png");
	mInfoBoard = std::make_unique<Sprite>(L"Data/Image/Menu/item_info_board.png");
}

void ItemSelect::Update(const Item* inventory)
{
	// SelectIndex����
	mInventory = inventory;
	int inventorySize = mInventory->GetItemNum();
	int oldSelectIndex = mSelectIndex;

	// �J�[�\���v�Z
	const int INDEX_MAX = inventorySize - 1;
	if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) ++mSelectIndex;
	if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  --mSelectIndex;
	if (mSelectIndex >= inventorySize) mSelectIndex = 0;
	if (mSelectIndex < 0) mSelectIndex = INDEX_MAX;

	if (Input::GetButtonTrigger(0, Input::BUTTON::UP))    mSelectIndex -= HORIZONTAL_NUM;
	if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN))  mSelectIndex += HORIZONTAL_NUM;
	if (mSelectIndex >= inventorySize)
	{
		int current = mSelectIndex / HORIZONTAL_NUM;
		int max = INDEX_MAX / HORIZONTAL_NUM;
		if (current == max) mSelectIndex = INDEX_MAX;
		else mSelectIndex = mSelectIndex % HORIZONTAL_NUM;
	}
	if (mSelectIndex < 0) mSelectIndex = Math::Min((INDEX_MAX) / HORIZONTAL_NUM * HORIZONTAL_NUM + (mSelectIndex + HORIZONTAL_NUM), INDEX_MAX);

	// �L�[�K�C�h
	KeyGuide::Instance().Add(KeyGuide::DPAD, L"�J�[�\���ړ�");

	// �A�C�e�����X�V
	UpdateInfo();

	// �T�E���h
	if (oldSelectIndex != mSelectIndex) Audio::SoundPlay((int)Sound::CURSOR_MOVE);
}

void ItemSelect::Render(const Vector2& boardOffset)
{
	if (mSelectIndex >= mInventory->GetItemNum()) mSelectIndex = mInventory->GetItemNum() - 1;

	// �{�[�h�`��
	mItemBoard->Render(boardOffset, Vector2::ONE, Vector2::ZERO, mItemBoard->GetSize());



	// �A�C�e���A�C�R���`��
	{
		// �I�������C���x���g���̃A�C�e����S���`��
		const float ICON_SCALE_SIZE = ICON_SCALE * ICON_SIZE;
		const Vector2 offset(ICON_OFFSET + boardOffset.x, ICON_OFFSET + boardOffset.y);
		const Vector2 scale(ICON_SCALE, ICON_SCALE);
		const Vector2 size(ICON_SIZE, ICON_SIZE);

		int itemNum = mInventory->GetItemNum();
		if (itemNum > 0)
		{
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
		const ItemData::BaseData* base = mInventory->GetItemParam(mSelectIndex);
		if (base) base->icon->Render(iconPos, iconScale, texPos, iconSize);

		// ����
		RenderSetInfo(infoBoardPos);
		mFont.Render();
	}

}

void ItemSelect::UpdateInfo()
{
	// �C���t�H�X�V
	// �O��̃A�C�e�������폜
	mInfo.clear();

	const ItemData::BaseData* base = mInventory->GetItemParam(mSelectIndex);
	if (!base)
	{
		mInfo.push_back(L"�A�C�e���������Ă܂���");
		return;
	}

	// ���O
	mInfo.push_back(base->name.c_str());

	// ���
	mInfo.push_back(base->info);
}

void ItemSelect::RenderSetInfo(const Vector2& infoBoardPos)
{
	Vector2 pos;// ���ƂŃ{�[�h�̍��W�𑫂�(���΍��W
	Vector2 center(0.0f, 0.0f);
	Vector2 scale(1.0f, 1.0f);
	
	const float NAME_OFFSET_Y = 28;
	for (size_t i = 0; i < mInfo.size(); ++i)
	{
		// �A�C�e�������A�A�C�e���̏�񂩂ŕ�����
		if (i == 0)
		{
			// ���O
			pos = infoBoardPos + Vector2(mInfoBoard->GetSize().x / 2.0f, NAME_OFFSET_Y);
			center = Vector2(0.5f, 0.0f);
			mFont.RenderSet(mInfo[i].c_str(), pos, center, Define::FONT_COLOR);
		}
		else
		{
			// ���

			// ���s�ɕ�����
			const wchar_t* text = mInfo[i].c_str();
			size_t size = mInfo[i].size();
			std::wstring str[INFO_LINE_MAX];
			int strIndex = 0;
			for (size_t i = 0; i < size; ++i)
			{
				str[strIndex] += text[i];
				if (mFont.GetWidth(str[strIndex].c_str()) >= INFO_WIDTH_MAX)
				{
					strIndex = Math::Min(strIndex + 1, INFO_LINE_MAX - 1);
				}
			}

			// �����_�[�Z�b�g
			pos = infoBoardPos + Vector2(INFO_TEXT_OFFSET_X, INFO_TEXT_OFFSET_Y);
			center = Vector2(0.0f, 0.0f);
			for (int i = 0; i < INFO_LINE_MAX; ++i)
			{
				pos.y += INFO_TEXT_ADD_Y * i;
				mFont.RenderSet(str[i].c_str(), pos, center, Define::FONT_COLOR);
			}
		}
	}
}

Vector2 ItemSelect::GetCursorRightUpPos() const
{
	const float ICON_SCALE_SIZE = ICON_SIZE * ICON_SCALE;
	float x = ICON_OFFSET + mSelectIndex % HORIZONTAL_NUM * ICON_SCALE_SIZE + ICON_SCALE_SIZE;
	float y = ICON_OFFSET + mSelectIndex / HORIZONTAL_NUM * ICON_SCALE_SIZE;
	return Vector2(x, y);
}
