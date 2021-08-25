#include "MenuEquipment.h"

#include "lib/Input.h"

#include "DataBase.h"
#include "Define.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Singleton.h"
#include "StatusData.h"

void MenuEquipment::Initialize(const PlayerManager* plm)
{
	int playerNum = plm->GetNum();

	// �摜�̓ǂݍ���
	mTypeBoard  = std::make_unique<Sprite>(L"Data/Image/Menu/equipment_type_board.png");
	mTypeSelect = std::make_unique<Sprite>(L"Data/Image/Menu/equipment_type_select.png");
	mCharacterSelect.Initialize(plm);
	mEquipmentSelect.Initialize();

	// �t�H���g�ɒǉ�
	mNameFont.Initialize();
	for (int i = 0; i < playerNum; ++i)
	{
		Status status = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(plm->GetPlayer(i)->GetCharaID());
		mNameFont.Add(status.GetName().c_str());
	}

	mFont.Initialize();
	mFont.Add(L"����F");
	mFont.Add(L"�h��F");
}

MenuBase::Select MenuEquipment::Update(PlayerManager* plm)
{
	mCharacterSelect.Update();

	// �I�𒆂̃L�����̃X�e�[�^�X(�����Q�Ɨp)�擾
	Player* selectPlayer = plm->GetPlayer(mCharacterSelect.GetIndex());
	int selectCharaID = selectPlayer->GetCharaID();
	Status status = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(selectCharaID);

	if (!mIsDecideType)
	{
		// �ǂ̃^�C�v�̑����i��ς��邩
		const int SELECT_MAX = EquipmentData::TYPE_MAX;
		if (Input::GetButtonTrigger(0, Input::BUTTON::UP))   mSelectIndex = (mSelectIndex + 1) % SELECT_MAX;
		if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mSelectIndex = (mSelectIndex + (SELECT_MAX - 1)) % SELECT_MAX;

		// �O�̉�ʂɖ߂�
		if (Input::GetButtonTrigger(0, Input::BUTTON::B))
		{
			return MenuBase::Select::BACK;
		}

		if (Input::GetButtonTrigger(0, Input::BUTTON::A))
		{
			mIsDecideType = true;
		}
	}
	else
	{
		// �I�񂾑�����index�擾
		EquipmentData::Type type = (EquipmentData::Type)mSelectIndex;
		EquipmentInventory* equipmentInventory = plm->GetEquipmentInventory();
		std::vector<EquipmentInventory::Data> inventory = equipmentInventory->GetInventory(type);
		int equipmentIndex = mEquipmentSelect.Update(inventory);
		
		// �I�񂾂�
		bool isEquip = true; // �������̑�����I�񂾎��O���悤
		if (equipmentIndex != -1)
		{
			// ���łɑ������Ă���l��������O��
			const Character* equipChara = inventory[equipmentIndex].equipChara;
			if (equipChara)
			{
				// �������O��
				Status equipCharaStatus = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(equipChara->GetCharaID());
				equipCharaStatus.GetEquipments()->UnEquip(inventory[equipmentIndex].equipmentID);
				Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(equipChara->GetCharaID(), equipCharaStatus);

				// �������̃L�����̑�������L�����������Ȃ�A�O������
				if (selectPlayer == equipChara)
				{
					isEquip = false;
				}

				// �Q�ƃL������null�ɂ���
				equipmentInventory->UnSetChara(type, equipChara);
			}

			if (isEquip)
			{
				// ���^�C�v�̂��̂𑕔����Ă�����O��
				status.GetEquipments()->UnEquip(type);
				equipmentInventory->UnSetChara(type, selectPlayer);

				// ��������
				status.GetEquipments()->Equip(selectPlayer, inventory[equipmentIndex].equipmentID);
				equipmentInventory->SetChara(type, selectPlayer, equipmentIndex);
				Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(selectCharaID, status);
			}

			mIsDecideType = false;
			mEquipmentSelect.ClearData();
		}

		// �O�̏�Ԃɖ߂�
		if (Input::GetButtonTrigger(0, Input::BUTTON::B))
		{
			mIsDecideType = false;
			mEquipmentSelect.ClearData();
		}
	}

	// �t�H���g�����_�[�ɒǉ�
	Vector2 boardPos(BOARD_POS_X, BOARD_POS_Y);
	Vector2 pos(boardPos.x + FONT_OFFSET_X, boardPos.y + FONT_OFFSET_Y);
	Vector2 center(Vector2::ZERO);
	{
		// ���O
		float nameX = boardPos.x + mTypeBoard->GetSize().x / 2.0f;
		float nameY = boardPos.y + FONT_OFFSET_Y;
		Vector2 namePos(nameX, nameY);
		Vector2 nameCenter(mNameFont.GetWidth(mCharacterSelect.GetIndex()) / 2.0f, 0.0f);
		mNameFont.RenderSet(mCharacterSelect.GetIndex(), namePos, nameCenter, Define::FONT_COLOR);


		// �����i
		const int WEAPON_INDEX = 0;
		const int ARMOR_INDEX = 1;

		// ����
		pos.y += FONT_ADD_Y;
		mFont.RenderSet(WEAPON_INDEX, pos, center, Define::FONT_COLOR);

		// ���햼
		const EquipmentData::Param* param = status.GetEquipments()->GetParam(EquipmentData::WEAPON);
		if (param)
		{
			mFont.RenderSet(param->name.c_str(), Vector2(pos.x + mFont.GetWidth(WEAPON_INDEX), pos.y), center, Define::FONT_COLOR);
		}

		// �h��
		pos.y += FONT_ADD_Y;
		mFont.RenderSet(ARMOR_INDEX, pos, center, Define::FONT_COLOR);

		// �h�
		param = status.GetEquipments()->GetParam(EquipmentData::ARMOR);
		if (param)
		{
			mFont.RenderSet(param->name.c_str(), Vector2(pos.x + mFont.GetWidth(ARMOR_INDEX), pos.y), center, Define::FONT_COLOR);
		}
	}

	return NONE;
}

void MenuEquipment::Render()
{
	Vector2 pos(BOARD_POS_X, BOARD_POS_Y);
	mCharacterSelect.Render(pos + Vector2(FONT_OFFSET_X, 0.0f));

	Vector2 scale(Vector2::ONE);
	Vector2 texPos(Vector2::ZERO);
	mTypeBoard->Render(pos, scale, texPos, mTypeBoard->GetSize());

	// �Z���N�g�摜�`��
	pos.x += FONT_OFFSET_X;
	pos.y += FONT_OFFSET_Y + (mSelectIndex + 1) * FONT_ADD_Y;
	mTypeSelect->Render(pos, scale, texPos, mTypeSelect->GetSize());

	// �����`��
	mNameFont.Render();
	mFont.Render();

	// �����ꗗ��`��
	if (mIsDecideType)
	{
		const float ADDJUST = 10.0f;
		const float OFFSET_X = mTypeBoard->GetSize().x + ADDJUST;
		Vector2 selectPos(BOARD_POS_X + OFFSET_X, BOARD_POS_Y);
		mEquipmentSelect.Render(selectPos);
	}
}

void MenuEquipment::Release()
{
	mCharacterSelect.Release();

	mNameFont.Release();
	mFont.Release();

}