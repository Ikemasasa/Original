#include "MenuCharacterSelect.h"

#include "lib/Audio.h"
#include "lib/Input.h"

#include "Define.h"
#include "KeyGuide.h"
#include "Player.h"
#include "PlayerManager.h"
#include "StatusData.h"

void MenuCharacterSelect::Initialize(const PlayerManager* plm)
{
	mBoard = std::make_unique<Sprite>(L"Data/Image/Menu/character_board.png");
	mFont.Initialize();
	mSelectIndex = 0;


	// ���O�̈ꕶ���ڂ��Z�b�g����
	for (size_t i = 0; i < plm->GetNum(); ++i)
	{
		const int LEN = 2;

		int charaID = plm->GetPlayer(i)->GetCharaID();
		wchar_t first = StatusData::GetPLStatus(charaID).GetName()[0];
		mPlayerNameFirst.push_back(first);
	}
}

void MenuCharacterSelect::Update()
{
	mOldSelectIndex = mSelectIndex;

	int max = mPlayerNameFirst.size();
	if (Input::GetButtonTrigger(0, Input::BUTTON::RB)) mSelectIndex = (mSelectIndex + 1) % max;
	if (Input::GetButtonTrigger(0, Input::BUTTON::LB)) mSelectIndex = (mSelectIndex + (max - 1)) % max;

	if (mOldSelectIndex != mSelectIndex) Audio::SoundPlay((int)Sound::CURSOR_MOVE);

	// �L�[�K�C�h
	if (max >= 2)
	{
		KeyGuide::Key key[] = { KeyGuide::RB, KeyGuide::LB };
		KeyGuide::Instance().Add(key, 2, L"�L�����N�^�[�I��");
	}

}

void MenuCharacterSelect::Render(Vector2 leftBottom)
{
	Vector2 scale(Vector2::ONE);
	Vector2 texPos(0.0f, 0.0f);
	Vector2 size(mBoard->GetSize());
	Vector2 center(0.0f, size.y); // leftBottom���󂯎�邽�� y = size.y;
	float angle = 0.0f;
	for (size_t i = 0; i < mPlayerNameFirst.size(); ++i)
	{
		Vector2 pos(leftBottom.x + size.x * i, leftBottom.y);
		Vector3 boardRGB = Vector3::ONE;
		Vector3 fontRGB = Vector3(Define::FONT_COLOR.x, Define::FONT_COLOR.y, Define::FONT_COLOR.z);
		if (i != mSelectIndex)
		{
			boardRGB *= 0.5f;
			fontRGB *= 0.5f;
		}

		// �v���[�g��`��
		mBoard->Render(pos, scale, texPos, size, center, angle, Vector4(boardRGB, 1.0f));

		// �t�H���g���Z�b�g
		float x = pos.x + FONT_OFFSET_X + ((mFont.GetFontSize() - mFont.GetWidth(mPlayerNameFirst[i])) / 2.0f);
		float y = pos.y - FONT_OFFSET_Y;
		Vector2 namePos(x, y); // �v���[�g��pos���������S������ y - offset
		mFont.RenderSet(mPlayerNameFirst[i], namePos, Vector2::ZERO, Vector4(fontRGB, 1.0f));
	}

	mFont.Render();
}

void MenuCharacterSelect::Release()
{
	mFont.Release();
}
