#include "DropItemShower.h"

#include "lib/Math.h"
#include "lib/Sprite.h"

#include "CameraManager.h"
#include "GameManager.h"
#include "ItemData.h"
#include "Singleton.h"

DropItemShower::DropItemShower()
{
	mBoard = std::make_unique<Sprite>(L"Data/Image/Menu/str_board.png");
	mFont.Initialize();
}

void DropItemShower::Add(const int dropItemID, const Vector3& dropCharaPos)
{
	DirectX::XMFLOAT4X4 view = Singleton<CameraManager>().GetInstance().GetView();
	DirectX::XMFLOAT4X4 proj = Singleton<CameraManager>().GetInstance().GetProj();
	Vector2 pos = dropCharaPos.WorldToScreen(view, proj);

	Data data = { dropItemID, pos, 0.0f , ALPHA_ADD, 0.0f, true};
	mDropItemDatas.emplace_back(data);
}

void DropItemShower::Update()
{
	for (auto it = mDropItemDatas.begin(); it != mDropItemDatas.end();)
	{
		bool isErase = false;

		it->pos.y -= VELOCITY_Y;
		if (it->fade)
		{
			it->alpha += it->alphaAdd;
			if (it->alpha >= 1.0f)
			{
				it->fade = false;
			}
			if (it->alpha <= 0.0f)
			{
				// データから消す
				it = mDropItemDatas.erase(it);
				isErase = true;
			}
		}
		else
		{
			it->dispTimer += GameManager::elapsedTime;
			if (it->dispTimer >= DISP_SEC)
			{
				it->fade = true;
				it->alphaAdd = -it->alphaAdd;
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
		const ItemData::BaseData* base = ItemData::GetBaseData(data.itemID);
		std::wstring str = L"ドロップ：" + base->name;

		center.x = mBoard->GetSize().x / 2.0f;
		mBoard->Render(data.pos, scale, Vector2::ZERO, mBoard->GetSize(), center, 0.0f, Vector4(1,1,1, Math::Clamp01(data.alpha)));

		center.x = 0.5f;
		color.w = data.alpha;
		mFont.RenderSet(str.c_str(), Vector2(data.pos.x, data.pos.y + STR_OFFSET_Y), center, color);
	}

	mFont.Render();
}
