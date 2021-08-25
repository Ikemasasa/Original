#include "CharacterHealth.h"

#include "lib/Input.h"
#include "lib/Sprite.h"

#include "DataBase.h"
#include "Define.h"
#include "PlayerManager.h"
#include "Singleton.h"
#include "StatusData.h"

void CharacterHealth::Initialize(const Vector2& leftTop)
{
	mHealthBoard = std::make_unique<Sprite>(L"Data/Image/Menu/health_board.png");
	mSelect = std::make_unique<Sprite>(L"Data/Image/Menu/health_board_select.png");

	mBoardLeftTop = leftTop;

	mFont.Initialize();
	mFontValue.Initialize();
}

void CharacterHealth::Update(const std::vector<Status>& statusArray)
{
	// �v���[�g�̕K�v���擾
	mBoardNum = statusArray.size();

	if (Input::GetButtonTrigger(0, Input::BUTTON::UP))   mSelectIndex = (mSelectIndex + (mBoardNum - 1)) % mBoardNum;
	if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mSelectIndex = (mSelectIndex + 1) % mBoardNum;

	// ���O, HP, MP�̃t�H���g�Z�b�g
	const wchar_t* statusName[STATUS_NUM] = { L"HP", L"MP" };
	for (int i = 0; i < mBoardNum; ++i)
	{
		float boardY = i * mHealthBoard->GetSize().y;
		float width = 0.0f;
		Vector2 pos = {};
		Vector2 center = {};

		// ���O
		width = mFont.GetWidth(statusArray[i].GetName().c_str());
		pos = Vector2(mBoardLeftTop.x + mHealthBoard->GetSize().x * 0.5f, mBoardLeftTop.y + FIRST_OFFSET_Y + boardY);
		center = Vector2(width * 0.5f, 0.0f);
		mFont.RenderSet(statusArray[i].GetName().c_str(), pos, center, Define::FONT_COLOR);

		// �X�e�[�^�X���l��z��
		int statusValue[STATUS_NUM] = { statusArray[i].GetHP(), statusArray[i].GetMP() };
		int statusMax[STATUS_NUM] = { statusArray[i].GetMaxHP(), statusArray[i].GetMaxMP() };
		for (int k = 0; k < STATUS_NUM; ++k)
		{
			// �n�߂̍��W�ݒ�
			pos.x = mBoardLeftTop.x + FIRST_OFFSET_X;
			pos.y = mBoardLeftTop.y + FIRST_OFFSET_Y + (ADD_OFFSET_Y * (k + 1)) + boardY;

			// �X�e�[�^�X��
			width = mFont.GetWidth(statusName[k]);
			mFont.RenderSet(statusName[k], pos, Vector2::ZERO, Define::FONT_COLOR);

			// ���݂̒l (cur)
			width = mFontValue.GetWidth(statusValue[k]);
			pos.x += CUR_OFFSET_X;
			center.x = width;
			mFontValue.RenderSet(statusValue[k], pos, center, Define::FONT_COLOR);

			// ��؂� (/)
			pos.x += DELIM_OFFSET_X;
			mFont.RenderSet(L"/ ", pos, Vector2::ZERO, Define::FONT_COLOR);

			// �ő�̒l (max)
			width = mFontValue.GetWidth(statusMax[k]);
			pos.x += MAX_OFFSET_X;
			center.x = width;
			mFontValue.RenderSet(statusMax[k], pos, center, Define::FONT_COLOR);
		}
	}
}

void CharacterHealth::Render(bool isSelectRender)
{
	Vector2 scale = Vector2::ONE;
	Vector2 texPos = Vector2::ZERO;
	Vector2 center = Vector2::ZERO;
	for (int i = 0; i < mBoardNum; ++i)
	{
		Vector2 boardPos(mBoardLeftTop.x, mBoardLeftTop.y + mHealthBoard->GetSize().y * i);

		mHealthBoard->Render(boardPos, scale, texPos, mHealthBoard->GetSize());
		if (isSelectRender && mSelectIndex == i)
		{
			mSelect->Render(boardPos - Vector2(SELECT_OFFSET, SELECT_OFFSET), scale, texPos, mSelect->GetSize());
		}
	}

	mFont.Render();
	mFontValue.Render();
}

void CharacterHealth::Release()
{
	mFont.Release();
	mFontValue.Release();
}
