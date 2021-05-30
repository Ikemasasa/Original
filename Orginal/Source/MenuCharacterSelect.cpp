#include "MenuCharacterSelect.h"

#include "lib/Input.h"
#include "lib/Texture.h"

#include "PlayerManager.h"
#include "Singleton.h"
#include "StatusData.h"
#include "DataBase.h"

MenuCharacterSelect::MenuCharacterSelect(PlayerManager* plm)
{
	mPlate = std::make_unique<Texture>(L"Data/Image/Menu/str_plate.png");
	mPlateSelect = std::make_unique<Texture>(L"Data/Image/Menu/plate_select.png");

	// –¼‘O‚ð‘Sˆõ•ª’Ç‰Á
	for(auto& pl : plm->GetPlayers())
	{
		std::wstring name = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(pl->GetCharaID()).name;
		mFont.Add(name.c_str());
	}
}

MenuCharacterSelect::~MenuCharacterSelect()
{
	mFont.Release();
}

void MenuCharacterSelect::Update()
{
	size_t playerNum = mFont.GetNum();

	if (Input::GetButtonTrigger(0, Input::BUTTON::UP))   mSelectIndex = (mSelectIndex + playerNum - 1) % playerNum;
	if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mSelectIndex = (mSelectIndex + 1) % playerNum;

	for (int i = 0; i < mFont.GetNum(); ++i)
	{
		Vector2 pos(PLATE_OFFSET_X + i * mPlate->GetSize().x, PLATE_OFFSET_Y);
		mFont.RenderSet(i, pos);
	}
}

void MenuCharacterSelect::Render()
{
	mFont.Render();
}
