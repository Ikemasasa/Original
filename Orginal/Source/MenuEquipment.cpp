#include "MenuEquipment.h"

#include "lib/Input.h"

#include "Define.h"
#include "Player.h"
#include "PlayerManager.h"
#include "StatusData.h"

void MenuEquipment::Initialize(const PlayerManager* plm)
{
	int playerNum = plm->GetNum();

	// 画像の読み込み
	mTypeBoard  = std::make_unique<Sprite>(L"Data/Image/Menu/equipment_type_board.png");
	mTypeSelect = std::make_unique<Sprite>(L"Data/Image/Menu/equipment_type_select.png");
	mCharacterSelect.Initialize(plm);
	mEquipmentSelect.Initialize();

	// フォントに追加
	mFont.Initialize();
	//for (int i = 0; i < playerNum; ++i)
	//{
	//	Status status = StatusData::GetPLStatus(plm->GetPlayer(i)->GetCharaID());
	//	mNameFont.Add(status.GetName().c_str());
	//}

	//mFont.Initialize();
	//mFont.Add(L"武器：");
	//mFont.Add(L"防具：");
}

MenuBase::Select MenuEquipment::Update(PlayerManager* plm)
{
	mCharacterSelect.Update();

	// 選択中のキャラのステータス(装備参照用)取得
	Player* selectPlayer = plm->GetPlayer(mCharacterSelect.GetIndex());
	int selectCharaID = selectPlayer->GetCharaID();
	Status status = StatusData::GetPLStatus(selectCharaID);

	if (!mIsDecideType)
	{
		// どのタイプの装備品を変えるか
		const int SELECT_MAX = EquipmentData::TYPE_NUM;
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
			AUDIO.SoundStop((int)Sound::SELECT);
			AUDIO.SoundPlay((int)Sound::EQUIP);

			// すでに装備している人がいたら外す
			const Character* equipChara = inventory[equipmentIndex].equipChara;
			if (equipChara)
			{
				// 装備を外す
				Status equipCharaStatus = StatusData::GetPLStatus(equipChara->GetCharaID());
				equipCharaStatus.GetEquipments()->UnEquip(inventory[equipmentIndex].equipmentID);
				StatusData::SetPLStatus(equipChara->GetCharaID(), equipCharaStatus);

				// 装備中のキャラと装備するキャラが同じなら、外すだけ
				if (selectPlayer == equipChara)
				{
					isEquip = false;
				}

				// 参照キャラをnullにする
				equipmentInventory->UnSetChara(type, equipChara);
			}

			if (isEquip)
			{
				// 同タイプのものを装備していたら外す
				status.GetEquipments()->UnEquip(type);
				equipmentInventory->UnSetChara(type, selectPlayer);

				// 装備する
				status.GetEquipments()->Equip(selectPlayer, inventory[equipmentIndex].equipmentID);
				equipmentInventory->SetChara(type, selectPlayer, equipmentIndex);
				StatusData::SetPLStatus(selectCharaID, status);
			}

			mIsDecideType = false;
			mEquipmentSelect.ClearData();
		}

		// 前の状態に戻る
		if (Input::GetButtonTrigger(0, Input::BUTTON::B))
		{
			mIsDecideType = false;
			mEquipmentSelect.ClearData();
		}
	}

	// フォントレンダーに追加
	Vector2 boardPos(BOARD_POS_X, BOARD_POS_Y);
	Vector2 pos(boardPos.x + FONT_OFFSET_X, boardPos.y + FONT_OFFSET_Y);
	Vector2 center(Vector2::ZERO);
	{
		// 名前
		float nameX = boardPos.x + mTypeBoard->GetSize().x / 2.0f;
		float nameY = boardPos.y + FONT_OFFSET_Y;
		Vector2 namePos(nameX, nameY);
		Vector2 nameCenter(0.5f, 0.0f);
		mFont.RenderSet(status.GetName().c_str(), namePos, nameCenter, Define::FONT_COLOR);


		// 装備品
		// 武器
		pos.y += FONT_ADD_Y;
		mFont.RenderSet(L"武器：", pos, center, Define::FONT_COLOR);

		// 武器名
		const EquipmentData::Param* param = status.GetEquipments()->GetParam(EquipmentData::WEAPON);
		if (param)
		{
			mFont.RenderSet(param->base->name.c_str(), Vector2(pos.x + mFont.GetWidth(L"武器："), pos.y), center, Define::FONT_COLOR);
		}

		// 防具
		pos.y += FONT_ADD_Y;
		mFont.RenderSet(L"防具：", pos, center, Define::FONT_COLOR);

		// 防具名
		param = status.GetEquipments()->GetParam(EquipmentData::ARMOR);
		if (param)
		{
			mFont.RenderSet(param->base->name.c_str(), Vector2(pos.x + mFont.GetWidth(L"防具："), pos.y), center, Define::FONT_COLOR);
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

	// セレクト画像描画
	pos.x += FONT_OFFSET_X;
	pos.y += FONT_OFFSET_Y + (mSelectIndex + 1) * FONT_ADD_Y;
	mTypeSelect->Render(pos, scale, texPos, mTypeSelect->GetSize());

	// 文字描画
	mFont.Render();

	// 装備一覧を描画
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
