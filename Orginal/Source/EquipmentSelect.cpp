#include "EquipmentSelect.h"

#include "lib/Audio.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Sprite.h"

#include "DataBase.h"
#include "EquipmentData.h"
#include "Singleton.h"

void EquipmentSelect::Initialize()
{
	mBoard = std::make_unique<Sprite>(L"Data/Image/board.png");
	mSelectFrame = std::make_unique<Sprite>(L"Data/Image/select_frame.png");
	mInfoBoard = std::make_unique<Sprite>(L"Data/Image/item_info_board.png");
}

int EquipmentSelect::Update(const std::vector<EquipmentInventory::Data>& equipmentsData)
{
	// SelectIndex����
	{
		mEquipmentsData = equipmentsData;
		int inventorySize = mEquipmentsData.size();

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

	if (Input::GetButtonTrigger(0, Input::BUTTON::A))
	{
		return mSelectIndex;
	}

	return -1;
}

void EquipmentSelect::Render(const Vector2& boardPos)
{

	// �{�[�h�`��
	mBoard->Render(boardPos, Vector2::ONE, Vector2::ZERO, mBoard->GetSize());

	//�A�C�R���`��
	{
		const float ICON_SCALE_SIZE = ICON_SCALE * ICON_SIZE;
		const Vector2 offset(ICON_OFFSET + boardPos.x, ICON_OFFSET + boardPos.y);
		const Vector2 scale(ICON_SCALE, ICON_SCALE);
		const Vector2 size(ICON_SIZE, ICON_SIZE);

		int num = mEquipmentsData.size();
		for (int i = 0; i < num; ++i)
		{
			float x = i % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x;
			float y = i / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y;
			Vector2 pos(x, y);

			const EquipmentData::Param* param = Singleton<DataBase>().GetInstance().GetEquipmentData()->GetParam(mEquipmentsData[mSelectIndex].equipmentID);
			param->icon->Render(pos, scale, Vector2::ZERO, size);
		}

		// �I���̃t���[���摜��`��
		Vector2 pos(mSelectIndex % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x, mSelectIndex / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y);
		mSelectFrame->Render(pos, scale, Vector2::ZERO, size);
	}

	// Info�`��
	{
		if (mEquipmentsData.empty()) return;
		const Vector2 texPos(0.0f, 0.0f);

		// �{�[�h
		const Vector2 infoBoardPos(boardPos.x + INFO_BOARD_OFFSET_X, boardPos.y);
		const Vector2 scale(1.0f, 1.0f);
		mInfoBoard->Render(infoBoardPos, scale, texPos, mInfoBoard->GetSize());

		// �A�C�e���A�C�R��
		const Vector2 iconPos(infoBoardPos.x + INFO_ICON_OFFSET_X, infoBoardPos.y + INFO_ICON_OFFSET_Y);
		const Vector2 iconScale(INFO_ICON_SCALE, INFO_ICON_SCALE);
		const Vector2 iconSize(ICON_SIZE, ICON_SIZE);

		const EquipmentData::Param* param = Singleton<DataBase>().GetInstance().GetEquipmentData()->GetParam(mEquipmentsData[mSelectIndex].equipmentID);
		param->icon->Render(iconPos, iconScale, Vector2::ZERO, iconSize);

		// ����
		RenderSetInfo(infoBoardPos);
		mFont.Render();
	}


}

void EquipmentSelect::RenderSetInfo(const Vector2& infoBoardPos)
{
	const float INFO_OFFSET_X = 32.0f;
	const float NAME_OFFSET_Y = 28;
	const float INFO_OFFSET_Y = 256.0f;
	const float INFO_ADD_Y = 46;

	// �����i�f�[�^�擾
	const EquipmentData::Param* param = Singleton<DataBase>().GetInstance().GetEquipmentData()->GetParam(mEquipmentsData[mSelectIndex].equipmentID);
	
	Vector2 scale(Vector2::ONE);
	Vector4 color(0.35f, 0.2f, 0.1f, 1.0f);

	// ���O
	Vector2 namePos = infoBoardPos + Vector2(mInfoBoard->GetSize().x / 2.0f, NAME_OFFSET_Y);
	Vector2 nameCenter(mFont.GetWidth(param->name.c_str()) / 2.0f, 0.0f);
	mFont.RenderSet(param->name.c_str(), namePos, nameCenter, scale, color);

	// �p�����[�^�[
	std::wstring str[PARAM_NUM] =
	{
		L"�U���́F" + std::to_wstring(param->atk),
		L"�h��́F" + std::to_wstring(param->def),
		L"�f�����F" + std::to_wstring(param->spd)
	};

	Vector2 pos;
	Vector2 center(Vector2::ZERO);
	for (int i = 0; i < PARAM_NUM; ++i)
	{
		pos = infoBoardPos + Vector2(INFO_OFFSET_X, INFO_OFFSET_Y + INFO_ADD_Y * i);
		mFont.RenderSet(str[i].c_str(), pos, center, scale, color);
	}
}