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

	// 画像の読み込み
	mBoard			 = std::make_unique<Sprite>(L"Data/Image/Menu/status_plate.png");
	mTypeSelect = std::make_unique<Sprite>(L"Data/Image/Menu/select.png");
	mCharacterSelect.Initialize(plm);
	mEquipmentSelect.Initialize();

	// フォントに追加
	mNameFont.Initialize();
	for (int i = 0; i < playerNum; ++i)
	{
		Status status = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(plm->GetPlayer(i)->GetCharaID());
		mNameFont.Add(status.name.c_str());
	}

	mFont.Initialize();
	mFont.Add(L"武器：");
	mFont.Add(L"防具：");
}

MenuBase::Select MenuEquipment::Update(PlayerManager* plm)
{
	mCharacterSelect.Update();

	// 選択中のキャラのステータス(装備参照用)取得
	Player* selectPlayer = plm->GetPlayer(mCharacterSelect.GetIndex());
	int selectCharaID = selectPlayer->GetCharaID();
	Status status = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(selectCharaID);

	if (!mIsDecideType)
	{
		// どのタイプの装備品を変えるか
		const int SELECT_MAX = EquipmentData::TYPE_MAX;
		if (Input::GetButtonTrigger(0, Input::BUTTON::UP))   mSelectIndex = (mSelectIndex + 1) % SELECT_MAX;
		if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mSelectIndex = (mSelectIndex + (SELECT_MAX - 1)) % SELECT_MAX;

		// 前の画面に戻る
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
		// 選んだ装備のindex取得
		EquipmentData::Type type = (EquipmentData::Type)mSelectIndex;
		EquipmentInventory* equipmentInventory = plm->GetEquipmentInventory();
		std::vector<EquipmentInventory::Data> inventory = equipmentInventory->GetInventory(type);
		int equipmentIndex = mEquipmentSelect.Update(inventory);
		
		// 選んだら
		bool isEquip = true; // 装備中の装備を選んだ時外すよう
		if (equipmentIndex != -1)
		{
			// すでに装備している人がいたら外す
			const Character* chara = inventory[equipmentIndex].equipmentChara;
			if (chara)
			{
				// 装備を外す
				Status equipCharaStatus = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(chara->GetCharaID());
				equipCharaStatus.equipments.UnEquip(inventory[equipmentIndex].equipmentID);
				Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(chara->GetCharaID(), equipCharaStatus);

				// 装備中のキャラの装備するキャラが同じなら、外すだけ
				if (selectPlayer == chara)
				{
					isEquip = false;
				}

				// 参照キャラをnullにする
				equipmentInventory->UnSetChara(type, equipmentIndex);
			}

			if (isEquip)
			{
				// 装備する
				status.equipments.Equip(selectPlayer, inventory[equipmentIndex].equipmentID);
				equipmentInventory->SetChara(type, selectPlayer, equipmentIndex);
				Singleton<DataBase>().GetInstance().GetStatusData()->SetPLStatus(selectCharaID, status);
			}

			mIsDecideType = false;
		}

		// 前の状態に戻る
		if (Input::GetButtonTrigger(0, Input::BUTTON::B))
		{
			mIsDecideType = false;
		}
	}

	// フォントレンダーに追加
	Vector2 boardPos(BOARD_POS_X, BOARD_POS_Y);
	Vector2 pos(boardPos.x + FIRST_OFFSET_X, boardPos.y + FIRST_OFFSET_Y);
	Vector2 scale(Vector2::ONE);
	Vector2 center(Vector2::ZERO);
	Vector4 color(0.4f, 0.2f, 0.0f, 1.0f);
	{
		// 名前
		float nameX = boardPos.x + mBoard->GetSize().x / 2.0f;
		float nameY = boardPos.y + FIRST_OFFSET_Y;
		Vector2 namePos(nameX, nameY);
		Vector2 nameCenter(mNameFont.GetWidth(mCharacterSelect.GetIndex()) / 2.0f, 0.0f);
		mNameFont.RenderSet(mCharacterSelect.GetIndex(), namePos, nameCenter, scale, color);


		// 装備品
		const int WEAPON_INDEX = 0;
		const int ARMOR_INDEX = 1;

		// 武器
		pos.y += ADD_OFFSET_Y;
		mFont.RenderSet(WEAPON_INDEX, pos, center, scale, color);

		// 武器名
		const EquipmentData::Param* param = status.equipments.GetParam(EquipmentData::WEAPON);
		if (param)
		{
			mFont.RenderSet(param->name.c_str(), Vector2(pos.x + mFont.GetWidth(WEAPON_INDEX), pos.y), center, scale, color);
		}

		// 防具
		pos.y += ADD_OFFSET_Y;
		mFont.RenderSet(ARMOR_INDEX, pos, center, scale, color);

		// 防具名
		param = status.equipments.GetParam(EquipmentData::ARMOR);
		if (param)
		{
			mFont.RenderSet(param->name.c_str(), Vector2(pos.x + mFont.GetWidth(ARMOR_INDEX), pos.y), center, scale, color);
		}
	}

	return NONE;
}

void MenuEquipment::Render()
{
	Vector2 pos(BOARD_POS_X, BOARD_POS_Y);
	mCharacterSelect.Render(pos);

	Vector2 scale(Vector2::ONE);
	Vector2 texPos(Vector2::ZERO);
	mBoard->Render(pos, scale, texPos, mBoard->GetSize());

	// セレクト画像描画
	pos.x += FIRST_OFFSET_X;
	pos.y += FIRST_OFFSET_Y + (mSelectIndex + 1) * ADD_OFFSET_Y;
	mTypeSelect->Render(pos, scale, texPos, mTypeSelect->GetSize());

	// 文字描画
	mNameFont.Render();
	mFont.Render();

	// 装備一覧を描画
	if (mIsDecideType)
	{
		const float ADDJUST = 10.0f;
		const float OFFSET_X = mBoard->GetSize().x + ADDJUST;
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
