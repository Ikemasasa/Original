#include "CharacterHealth.h"

#include "lib/Input.h"
#include "lib/Sprite.h"

#include "PlayerManager.h"
#include "Singleton.h"
#include "StatusData.h"
#include "DataBase.h"

void CharacterHealth::Initialize(const Vector2& leftTop)
{
	mHealthPlate = std::make_unique<Sprite>(L"Data/Image/Menu/health_plate.png");
	mSelect = std::make_unique<Sprite>(L"Data/Image/Menu/health_plate_select.png");

	mPlateLeftTop = leftTop;

	mPlNameFont.Initialize();
	mStatusNameFont.Initialize();
	mDelimFont.Initialize();
	mFontValue.Initialize();

	mStatusNameFont.Add(L"HP");
	mStatusNameFont.Add(L"MP");
	mDelimFont.Add(L"/");
}

void CharacterHealth::Update(const std::vector<Status>& statusArray)
{
	if (Input::GetButtonTrigger(0, Input::BUTTON::RB)) mSelectIndex = (mSelectIndex + 1) % mHealthPlates.size();
	if (Input::GetButtonTrigger(0, Input::BUTTON::LB)) mSelectIndex = (mSelectIndex + (mHealthPlates.size() - 1)) % mHealthPlates.size();

	// 名前, HP, MPのフォントセット
		// health_plate作成
	std::vector<int> curValue;
	std::vector<int> maxValue;
	const Vector2 scale(Vector2::One());
	const Vector4 color(0.77f, 0.67f, 0.42f, 1.0f); // フォントの色
	size_t statusNum = statusArray.size();
	mHealthPlates.resize(statusNum);
	for (size_t i = 0; i < statusNum; ++i)
	{
		// TODO : 追加されてないなら追加する(正直、initializeでやるべき)
		if(mPlNameFont.Find(statusArray[i].name.c_str()) == false) mPlNameFont.Add(statusArray[i].name.c_str());

		mHealthPlates[i].name = statusArray[i].name;
		curValue.emplace_back(statusArray[i].hp);
		curValue.emplace_back(statusArray[i].mp);
		maxValue.emplace_back(statusArray[i].maxHP);
		maxValue.emplace_back(statusArray[i].maxMP);
	}

	for (size_t i = 0; i < mHealthPlates.size(); ++i)
	{
		float offsetY = i * mHealthPlate->GetSize().y;

		for (int k = 0; k < VERTICAL_NUM; ++k)
		{
			float width = 0.0f;
			Vector2 pos = {};
			Vector2 center = {};

			if (k == 0) // 名前
			{
				width = mPlNameFont.GetWidth(mHealthPlates[i].name.c_str());
				pos = Vector2(mPlateLeftTop.x + mHealthPlate->GetSize().x * 0.5f, mPlateLeftTop.y + HealthPlate::FIRST_OFFSET_Y + offsetY);
				center = Vector2(width * 0.5f, 0.0f);
				mPlNameFont.RenderSet(i, pos, center, scale, color);
			}
			else // HP, MP
			{
				pos = Vector2(mPlateLeftTop.x + HealthPlate::FIRST_OFFSET_X, mPlateLeftTop.y + HealthPlate::FIRST_OFFSET_Y + HealthPlate::ADD_OFFSET_Y * k + offsetY);

				int statusNameIndex = k - 1;
				int statusIndex = (k - 1) + (i * (VERTICAL_NUM - 1)); // - 1 : 名前の分 

				// ステータス名
				width = mStatusNameFont.GetWidth(statusNameIndex);
				mStatusNameFont.RenderSet(statusNameIndex, pos, Vector2::Zero(), scale, color);

				// 現在の値 (cur)
				width = mFontValue.GetWidth(curValue[statusIndex]);
				pos.x += HealthPlate::CUR_OFFSET_X;
				center.x = width;
				mFontValue.RenderSet(curValue[statusIndex], pos, center, scale, color);

				// 区切り (/)
				pos.x += HealthPlate::DELIM_OFFSET_X;
				mDelimFont.RenderSet(0, pos, Vector2::Zero(), scale, color);

				// 最大の値 (max)
				width = mFontValue.GetWidth(maxValue[statusIndex]);
				pos.x += HealthPlate::DELIM_OFFSET_X + HealthPlate::MAX_OFFSET_X;
				center.x = width;
				mFontValue.RenderSet(maxValue[statusIndex], pos, center, scale, color);
			}
		}
	}
}

void CharacterHealth::Render(bool isSelectRender)
{
	Vector2 scale = Vector2::One();
	Vector2 texPos = Vector2::Zero();
	Vector2 center = Vector2::Zero();
	float angle = 0.0f;
	for (size_t i = 0; i < mHealthPlates.size(); ++i)
	{
		Vector2 platePos(mPlateLeftTop.x, mPlateLeftTop.y + mHealthPlate->GetSize().y * i);

		mHealthPlate->Render(platePos, scale, texPos, mHealthPlate->GetSize());
		if (isSelectRender && mSelectIndex == i)
		{
			Vector4 selectColor(0.8f, 0.7f, 0.5f, 1.0f);
			mSelect->Render(platePos, scale, texPos, mSelect->GetSize(), center, angle, selectColor);
		}
	}

	mPlNameFont.Render();
	mStatusNameFont.Render();
	mDelimFont.Render();
	mFontValue.Render();
}

void CharacterHealth::Release()
{
	mPlNameFont.Release();
	mStatusNameFont.Release();
	mDelimFont.Release();
	mFontValue.Release();
}
