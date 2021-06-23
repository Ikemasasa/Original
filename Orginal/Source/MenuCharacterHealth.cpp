#include "MenuCharacterHealth.h"

#include "lib/Input.h"
#include "lib/Sprite.h"

#include "PlayerManager.h"
#include "Singleton.h"
#include "StatusData.h"
#include "DataBase.h"

void MenuCharacterHealth::Initialize(const PlayerManager* plm, const Vector2& leftTop)
{
	mHealthPlate = std::make_unique<Sprite>(L"Data/Image/Menu/health_plate.png");
	mSelect = std::make_unique<Sprite>(L"Data/Image/Menu/health_plate_select.png");

	mPlateLeftTop = leftTop;

	mPlNameFont.Initialize();
	mStatusNameFont.Initialize();
	mDelimFont.Initialize();
	mFontValue.Initialize();

	// health_plate作成
	std::vector<int> curValue;
	std::vector<int> maxValue;

	size_t playerNum = plm->GetNum();
	mHealthPlates.resize(playerNum);
	for (size_t i = 0; i < playerNum; ++i)
	{
		const auto& pl = plm->GetPlayer(i);
		Status plStatus = *pl->GetStatus();

		mPlNameFont.Add(plStatus.name.c_str());
		mHealthPlates[i].name = plStatus.name;
		curValue.emplace_back(plStatus.hp);
		curValue.emplace_back(plStatus.mp);
		maxValue.emplace_back(plStatus.maxHP);
		maxValue.emplace_back(plStatus.maxMP);
	}

	mStatusNameFont.Add(L"HP");
	mStatusNameFont.Add(L"MP");
	mDelimFont.Add(L"/");

	// 名前, HP, MPのフォントセット
	for (int i = 0; i < mHealthPlates.size(); ++i)
	{
		for (int k = 0; k < VERTICAL_NUM; ++k)
		{
			float width = 0.0f;
			Vector2 pos = {};
			Vector2 center = {};

			if (k == 0) // 名前
			{
				width = mPlNameFont.GetWidth(mHealthPlates[i].name.c_str());
				pos = Vector2(leftTop.x + mHealthPlate->GetSize().x * 0.5f, leftTop.y + HealthPlate::FIRST_OFFSET_Y);
				center = Vector2(width * 0.5f, 0.0f);
				mPlNameFont.RenderSet(i, pos, center);
			}
			else // HP, MP
			{
				pos = Vector2(leftTop.x + HealthPlate::FIRST_OFFSET_X, leftTop.y + HealthPlate::FIRST_OFFSET_Y + HealthPlate::ADD_OFFSET_Y * k);

				int statusIndex = k - 1;

				// ステータス名
				width = mStatusNameFont.GetWidth(statusIndex);
				mStatusNameFont.RenderSet(statusIndex, pos); // k - 1 : 名前の分 

				// 現在の値 (cur)
				width = mFontValue.GetWidth(curValue[statusIndex]);
				pos.x += HealthPlate::CUR_OFFSET_X;
				center.x = width;
				mFontValue.RenderSet(curValue[statusIndex], pos, center);

				// 区切り (/)
				pos.x += HealthPlate::DELIM_OFFSET_X;
				mDelimFont.RenderSet(0, pos);

				// 最大の値 (max)
				width = mFontValue.GetWidth(maxValue[statusIndex]);
				pos.x += HealthPlate::DELIM_OFFSET_X + HealthPlate::MAX_OFFSET_X;
				center.x = width;
				mFontValue.RenderSet(maxValue[statusIndex], pos, center);
			}
		}
	}
}

void MenuCharacterHealth::Update()
{
	if (Input::GetButtonTrigger(0, Input::BUTTON::RB)) mSelectIndex = (mSelectIndex + 1) % mHealthPlates.size();
	if (Input::GetButtonTrigger(0, Input::BUTTON::LB)) mSelectIndex = (mSelectIndex + (mHealthPlates.size() - 1)) % mHealthPlates.size();
}

void MenuCharacterHealth::Render(bool isSelectRender)
{
	Vector2 scale = Vector2::One();
	Vector2 texPos = Vector2::Zero();
	Vector2 center = Vector2::Zero();
	float angle = 0.0f;
	for (size_t i = 0; i < mHealthPlates.size(); ++i)
	{
		Vector2 platePos(mPlateLeftTop.x, mPlateLeftTop.y + mPlateLeftTop.y * i);

		mHealthPlate->Render(platePos, scale, texPos, mHealthPlate->GetSize());
		if (isSelectRender && mSelectIndex == i)
		{
			Vector4 selectColor(0.8f, 0.7f, 0.5f, 1.0f);
			mSelect->Render(platePos, scale, texPos, mSelect->GetSize(), center, angle, selectColor);
		}
	}

	mPlNameFont.Render(false);
	mStatusNameFont.Render(false);
	mDelimFont.Render(false);
	mFontValue.Render(false);
}

void MenuCharacterHealth::Release()
{
	mPlNameFont.Release();
	mStatusNameFont.Release();
	mDelimFont.Release();
	mFontValue.Release();
}
