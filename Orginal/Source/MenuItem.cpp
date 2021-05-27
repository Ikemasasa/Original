#include "MenuItem.h"

#include <map>

#include "lib/ConvertString.h"
#include "lib/Input.h"
#include "lib/Math.h"
#include "lib/Texture.h"

#include "ItemData.h"
#include "PlayerManager.h"
#include "Singleton.h"


MenuItem::MenuItem(PlayerManager* plm)
{
	mPlayerManager = plm;

	mSelect = std::make_unique<Texture>(L"Data/Image/select_frame.png");
	mBoard = std::make_unique<Texture>(L"Data/Image/board.png");

	mFont.Initialize(64, 64);
}

MenuItem::~MenuItem()
{
	mFont.Release();
}

void MenuItem::Update()
{
	if (Input::GetButtonTrigger(0, Input::BUTTON::RIGHT)) ++mSelectIndex;
	if (Input::GetButtonTrigger(0, Input::BUTTON::LEFT))  --mSelectIndex;
	if (Input::GetButtonTrigger(0, Input::BUTTON::UP))    mSelectIndex -= HORIZONTAL_NUM;
	if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN))  mSelectIndex += HORIZONTAL_NUM;

	mSelectIndex = Math::Clamp(mSelectIndex, 0, 8);
}

void MenuItem::Render()
{
	mBoard->Render(Vector2(BOARD_OFFSET_X, BOARD_OFFSET_Y), Vector2::One(), Vector2::Zero(), mBoard->GetSize());

	RenderIcons();

	mFont.Render();
} 

void MenuItem::Release()
{

}

void MenuItem::RenderIcons()
{
	// アイテムアイコン描画
	const float ICON_SCALE_SIZE = ICON_SCALE * ICON_SIZE;
	const Vector2 offset(ICON_OFFSET + BOARD_OFFSET_X, ICON_OFFSET + BOARD_OFFSET_Y);
	const Vector2 scale(ICON_SCALE, ICON_SCALE);
	const Vector2 size(ICON_SIZE, ICON_SIZE);

	const std::list<std::shared_ptr<Player>>& players = mPlayerManager->GetPlayers();
	int totalNum = 0;
	for (auto& pl : players)
	{
		const std::map<int, int> inventory = pl->GetInventory()->GetAll();
		for (auto& item : inventory)
		{
			int id = item.first;
			int num = item.second;

			std::shared_ptr<Texture> icon = Singleton<DataBase>().GetInstance().GetItemData()->GetItemParam(id).icon;

			for (int i = 0; i < num; ++i)
			{
				float x = (totalNum + i) % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x;
				float y = (totalNum + i) / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y;
				Vector2 pos(x, y);
				icon->Render(pos, scale, Vector2::Zero(), size);
			}

			totalNum += num;
		}
	}

	// 選択のフレーム画像を描画
	{
		Vector2 pos(mSelectIndex % HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.x, mSelectIndex / HORIZONTAL_NUM * ICON_SCALE_SIZE + offset.y);
		mSelect->Render(pos, scale, Vector2::Zero(), size);
	}
}