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
	mBoard = std::make_unique<Sprite>(L"Data/Image/Menu/character_board.png");
	mFont.Initialize();
	mCharacterNum = 0;
	mSelectIndex = 0;


	// 名前の一文字目をセットする
	for (size_t i = 0; i < plm->GetNum(); ++i)
	{
		const int LEN = 2;
		wchar_t name[LEN] = {};

		int charaID = plm->GetPlayer(i)->GetCharaID();
		name[0] = Singleton<DataBase>().GetInstance().GetStatusData()->GetPLStatus(charaID).GetName()[0];

		mFont.Add(name);
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
	Vector2 scale(Vector2::ONE);
	Vector2 texPos(0.0f, 0.0f);
	Vector2 size(mBoard->GetSize());
	Vector2 center(0.0f, size.y); // leftBottomを受け取るため y = size.y;
	float angle = 0.0f;
	for (int i = 0; i < mCharacterNum; ++i)
	{
		Vector2 pos(leftBottom.x + size.x * i, leftBottom.y);
		Vector3 boardRGB = Vector3::ONE;
		Vector3 fontRGB = Vector3(Define::FONT_COLOR.x, Define::FONT_COLOR.y, Define::FONT_COLOR.z);
		if (i != mSelectIndex)
		{
			boardRGB *= 0.5f;
			fontRGB *= 0.5f;
		}

		// プレートを描画
		mBoard->Render(pos, scale, texPos, size, center, angle, Vector4(boardRGB, 1.0f));

		// フォントを描画
		Vector2 namePos(pos.x + FONT_OFFSET_X, pos.y - FONT_OFFSET_Y); // プレートのposが左下中心だから y - offset
		mFont.RenderSet(i, namePos, Vector2::ZERO, Vector4(fontRGB, 1.0f));
	}

	mFont.Render();
}

void MenuCharacterSelect::Release()
{
	mFont.Release();
}
