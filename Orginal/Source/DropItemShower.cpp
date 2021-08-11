#include "DropItemShower.h"

#include "lib/Sprite.h"

#include "CameraManager.h"
#include "DataBase.h"
#include "EquipmentData.h"
#include "Singleton.h"

DropItemShower::DropItemShower()
{
	mBoard = std::make_unique<Sprite>(L"Data/Image/Menu/str_plate.png");
}

void DropItemShower::Add(const int dropItemID, const Vector3& dropCharaPos)
{
	DirectX::XMFLOAT4X4 view = Singleton<CameraManager>().GetInstance().GetView();
	DirectX::XMFLOAT4X4 proj = Singleton<CameraManager>().GetInstance().GetProj();
	Vector2 pos = dropCharaPos.WorldToScreen(view, proj);

	Data data = { dropItemID, pos, 0.0f , false};
	mDropItemDatas.emplace_back(data);
}

void DropItemShower::Update()
{

	for (auto it = mDropItemDatas.begin(); it != mDropItemDatas.end();)
	{
		it->pos.y -= VELOCITY_Y;
		
		bool isErase = false;
		if (!it->alphaTurn)
		{
			it->alpha += ALPHA_ADD;
			if (it->alpha >= ALPHA_TURN_VALUE)
			{
				it->alphaTurn = true;
			}
		}
		else 
		{
			it->alpha -= ALPHA_ADD;
			if (it->alpha <= 0.0f)
			{
				// fontから文字データを消す
				const EquipmentData::Param* param = Singleton<DataBase>().GetInstance().GetEquipmentData()->GetParam(it->itemID);
				mFont.Remove(param->name.c_str());

				// データから消す
				it = mDropItemDatas.erase(it);
				isErase = true;
			}
		}

		if (!isErase) ++it;
	}
}

void DropItemShower::Render()
{
	Vector2 center(Vector2::ZERO);
	Vector2 scale(Vector2::ONE);
	Vector4 color(0.15f, 0.15f, 0.15f, 0.0f);
	for (auto& data : mDropItemDatas)
	{
		const EquipmentData::Param* param = Singleton<DataBase>().GetInstance().GetEquipmentData()->GetParam(data.itemID);

		center.x = mBoard->GetSize().x / 2.0f;
		mBoard->Render(data.pos, Vector2(BOARD_SCALE, BOARD_SCALE), Vector2::ZERO, mBoard->GetSize(), center, 0.0f, Vector4(1,1,1,data.alpha));

		center.x = mFont.GetWidth(param->name.c_str()) / 2.0f;
		color.w = data.alpha;
		mFont.RenderSet(param->name.c_str(), Vector2(data.pos.x, data.pos.y + STR_OFFSET_Y), center, scale, color);
	}

	mFont.Render();
}
