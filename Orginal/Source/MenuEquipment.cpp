#include "MenuEquipment.h"

#include "lib/Input.h"

#include "Define.h"
#include "Player.h"
#include "PlayerManager.h"
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
	mFont.Initialize();
	//for (int i = 0; i < playerNum; ++i)
	//{
	//	Status status = StatusData::GetPLStatus(plm->GetPlayer(i)->GetCharaID());
	//	mNameFont.Add(status.GetName().c_str());
	//}

	//mFont.Initialize();
	//mFont.Add(L"����F");
	//mFont.Add(L"�h��F");
}

MenuBase::Select MenuEquipment::Update(PlayerManager* plm)
{
	mCharacterSelect.Update();

	// �I�𒆂̃L�����̃X�e�[�^�X(�����Q�Ɨp)�擾
	Player* selectPlayer = plm->GetPlayer(mCharacterSelect.GetIndex());
	int selectCharaID = selectPlayer->GetCharaID();
	Status status = StatusData::GetPLStatus(selectCharaID);

	if (!mIsDecideType)
	{
		// �ǂ̃^�C�v�̑����i��ς��邩
		const int SELECT_MAX = EquipmentData::TYPE_NUM;
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
			AUDIO.SoundStop((int)Sound::SELECT);
			AUDIO.SoundPlay((int)Sound::EQUIP);

			// ���łɑ������Ă���l��������O��
			const Character* equipChara = inventory[equipmentIndex].equipChara;
			if (equipChara)
			{
				// �������O��
				Status equipCharaStatus = StatusData::GetPLStatus(equipChara->GetCharaID());
				equipCharaStatus.GetEquipments()->UnEquip(inventory[equipmentIndex].equipmentID);
				StatusData::SetPLStatus(equipChara->GetCharaID(), equipCharaStatus);

				// �������̃L�����Ƒ�������L�����������Ȃ�A�O������
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
				StatusData::SetPLStatus(selectCharaID, status);
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
		Vector2 nameCenter(0.5f, 0.0f);
		mFont.RenderSet(status.GetName().c_str(), namePos, nameCenter, Define::FONT_COLOR);


		// �����i
		// ����
		pos.y += FONT_ADD_Y;
		mFont.RenderSet(L"����F", pos, center, Define::FONT_COLOR);

		// ���햼
		const EquipmentData::Param* param = status.GetEquipments()->GetParam(EquipmentData::WEAPON);
		if (param)
		{
			mFont.RenderSet(param->base->name.c_str(), Vector2(pos.x + mFont.GetWidth(L"����F"), pos.y), center, Define::FONT_COLOR);
		}

		// �h��
		pos.y += FONT_ADD_Y;
		mFont.RenderSet(L"�h��F", pos, center, Define::FONT_COLOR);

		// �h�
		param = status.GetEquipments()->GetParam(EquipmentData::ARMOR);
		if (param)
		{
			mFont.RenderSet(param->base->name.c_str(), Vector2(pos.x + mFont.GetWidth(L"�h��F"), pos.y), center, Define::FONT_COLOR);
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

	mFont.Release();
}
