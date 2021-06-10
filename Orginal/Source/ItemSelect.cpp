#include "ItemSelect.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "Item.h"

void ItemSelect::Initialize()
{
	mSelectIndex = 0;
	mOldSelectIndex = 0;

	mBoard = std::make_unique<Sprite>(L"Data/Image/board.png");
	mSelectFrame = std::make_unique<Sprite>(L"Data/Image/select_frame.png");
}

int ItemSelect::Update(const Item* inventory)
{
	// SelectIndex����
	{
		mInventory = inventory;
		int inventorySize = mInventory->GetAll().size();

		if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) ++mSelectIndex;
		if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  --mSelectIndex;
		if (mSelectIndex >= inventorySize) mSelectIndex -= inventorySize;
		if (mSelectIndex < 0) mSelectIndex += inventorySize;

		if (Input::GetButtonTrigger(0, Input::BUTTON::UP))    mSelectIndex -= HORIZONTAL_NUM;
		if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN))  mSelectIndex += HORIZONTAL_NUM;
		if (mSelectIndex >= inventorySize) mSelectIndex -= inventorySize + 1;
		if (mSelectIndex < 0) mSelectIndex += inventorySize + 1;
		mSelectIndex = Math::Clamp(mSelectIndex, 0, inventorySize - 1); // ���K����񂩂�������clamp���Ă�

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
	mBoard->Render(boardOffset, Vector2::One(), Vector2::Zero(), mBoard->GetSize());


	// �A�C�e���A�C�R���`��
	{
		// �I�������C���x���g���̃A�C�e����S���`��
		const float ICON_SCALE_SIZE = ICON_SCALE * ICON_SIZE;
		const Vector2 offset(ICON_OFFSET + boardOffset.x, ICON_OFFSET + boardOffset.y);
		const Vector2 scale(ICON_SCALE, ICON_SCALE);
		const Vector2 size(ICON_SIZE, ICON_SIZE);

		const std::vector<ItemData::ItemParam>& inventory = mInventory->GetAll();
		size_t itemNum = inventory.size();
		for (size_t i = 0; i < itemNum; ++i)
		{
			float x = i % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x;
			float y = i / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y;
			Vector2 pos(x, y);
			inventory[i].icon->Render(pos, scale, Vector2::Zero(), size);
		}

		// �I���̃t���[���摜��`��
		Vector2 pos(mSelectIndex % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x, mSelectIndex / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y);
		mSelectFrame->Render(pos, scale, Vector2::Zero(), size);
	}
}
