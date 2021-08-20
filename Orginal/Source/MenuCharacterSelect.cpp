#include "MenuCharacterSelect.h"

#include "lib/Audio.h"
#include "lib/Input.h"

#include "DataBase.h"
#include "Define.h"
#include "Player.h"
#include "PlayerManager.h"
#include "Singleton.h"
#include "StatusData.h"

void MenuCharacterSelect::Initialize(const PlayerManager* plm)
{
	mCharacterNum = 0;
	mSelectIndex = 0;

	mNameFont.Initialize();

	mPlate = std::make_unique<Sprite>(L"Data/Image/Menu/character_plate.png");

	// 名前の一文字目を作る
	for (size_t i = 0; i < plm->GetNum(); ++i)
	{
		const int LEN = 2;
		wchar_t name[LEN] = {};

		int charaID = plm->GetPlayer(i)->GetCharaID();
		name[0] = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(charaID).GetName()[0];

		mNameFont.Add(name);
		++mCharacterNum;
	}
}

void MenuCharacterSelect::Update()
{
	mOldSelectIndex = mSelectIndex;
	if (Input::GetButtonTrigger(0, Input::BUTTON::RB)) mSelectIndex = (mSelectIndex + 1) % mCharacterNum;
	if (Input::GetButtonTrigger(0, Input::BUTTON::LB)) mSelectIndex = (mSelectIndex + (mCharacterNum - 1)) % mCharacterNum;

	if (mOldSelectIndex != mSelectIndex) AUDIO.SoundPlay((int)Sound::CURSOR_MOVE);
}

void MenuCharacterSelect::Render(Vector2 leftBottom)
{
	const float PLATE_SCALE = 0.5f;
	const Vector2 FONT_OFFSET(5.0f, 32.0f);

	Vector2 scale(PLATE_SCALE, PLATE_SCALE);
	Vector2 texPos(0.0f, 0.0f);
	Vector2 size(mPlate->GetSize());
	Vector2 center(0.0f, size.y); // leftBottomを受け取るため y = size.y;
	float angle = 0.0f;
	for (int i = 0; i < mCharacterNum; ++i)
	{
		Vector2 pos(leftBottom.x + (size.x * scale.x * i), leftBottom.y);
		Vector3 plateRGB = Vector3::ONE;
		Vector3 fontRGB = Vector3(Define::FONT_COLOR.x, Define::FONT_COLOR.y, Define::FONT_COLOR.z);
		if (i != mSelectIndex)
		{
			plateRGB *= 0.5f;
			fontRGB *= 0.5f;
		}

		mPlate->Render(pos, scale, texPos, size, center, angle, Vector4(plateRGB, 1.0f));

		Vector2 namePos(pos.x + FONT_OFFSET.x, pos.y - FONT_OFFSET.y); // プレートのposが左下中心だから y - offset
		mNameFont.RenderSet(i, namePos, Vector2::ZERO, Vector4(fontRGB, 1.0f));
	}

	mNameFont.Render();
}

void MenuCharacterSelect::Release()
{
	mNameFont.Release();
}
