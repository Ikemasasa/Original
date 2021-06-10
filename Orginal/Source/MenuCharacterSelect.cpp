#include "MenuCharacterSelect.h"

#include "lib/Audio.h"
#include "lib/Input.h"

#include "PlayerManager.h"

void MenuCharacterSelect::Initialize(const PlayerManager* plm)
{
	mCharacterNum = 0;
	mSelectIndex = 0;

	mNameFont.Initialize();

	mPlate = std::make_unique<Sprite>(L"Data/Image/Menu/character_plate.png");

	// 名前の一文字目を作る
	const std::vector<std::shared_ptr<Player>>& players =  plm->GetPlayers();
	for (auto& pl : players)
	{
		const int LEN = 2;
		wchar_t name[LEN] = {};
		name[0] = pl->GetStatus()->name[0];

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
	Vector4 defaultColor(0.5f, 0.5f, 0.5f, 1.0f);
	Vector4 SelectColor(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < mCharacterNum; ++i)
	{
		Vector2 pos(leftBottom.x + (size.x * scale.x * i), leftBottom.y);
		Vector4 color = defaultColor;
		if (i == mSelectIndex) color = SelectColor;

		mPlate->Render(pos, scale, texPos, size, center, angle, color);

		Vector2 namePos(pos.x + FONT_OFFSET.x, pos.y - FONT_OFFSET.y); // プレートのposが左下中心だから y - offset
		mNameFont.RenderSet(i, namePos, Vector2::Zero(), Vector2::One(), color);
	}

	mNameFont.Render();
}

void MenuCharacterSelect::Release()
{
	mNameFont.Release();
}
