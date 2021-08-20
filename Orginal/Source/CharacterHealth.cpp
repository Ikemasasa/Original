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
	mHealthPlate = std::make_unique<Sprite>(L"Data/Image/Menu/health_plate.png");
	mSelect = std::make_unique<Sprite>(L"Data/Image/Menu/health_plate_select.png");

	mPlateLeftTop = leftTop;

	mFont.Initialize();
	mFontValue.Initialize();
}

void CharacterHealth::Update(const std::vector<Status>& statusArray)
{
	// �v���[�g�̕K�v���擾
	mPlateNum = statusArray.size();

	if (Input::GetButtonTrigger(0, Input::BUTTON::UP))   mSelectIndex = (mSelectIndex + (mPlateNum - 1)) % mPlateNum;
	if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mSelectIndex = (mSelectIndex + 1) % mPlateNum;

	// ���O, HP, MP�̃t�H���g�Z�b�g
	const wchar_t* statusName[STATUS_NUM] = { L"HP", L"MP" };
	for (size_t i = 0; i < mPlateNum; ++i)
	{
		float plateY = i * mHealthPlate->GetSize().y;
		float width = 0.0f;
		Vector2 pos = {};
		Vector2 center = {};

		// ���O
		width = mFont.GetWidth(statusArray[i].GetName().c_str());
		pos = Vector2(mPlateLeftTop.x + mHealthPlate->GetSize().x * 0.5f, mPlateLeftTop.y + FIRST_OFFSET_Y + plateY);
		center = Vector2(width * 0.5f, 0.0f);
		mFont.RenderSet(statusArray[i].GetName().c_str(), pos, center, Define::FONT_COLOR);

		// �X�e�[�^�X���l��z��
		int statusValue[STATUS_NUM] = { statusArray[i].GetHP(), statusArray[i].GetMP() };
		int statusMax[STATUS_NUM] = { statusArray[i].GetMaxHP(), statusArray[i].GetMaxMP() };
		for (size_t k = 0; k < STATUS_NUM; ++k)
		{
			// �n�߂̍��W�ݒ�
			pos.x = mPlateLeftTop.x + FIRST_OFFSET_X;
			pos.y = mPlateLeftTop.y + FIRST_OFFSET_Y + (ADD_OFFSET_Y * (k + 1)) + plateY;

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
	for (size_t i = 0; i < mPlateNum; ++i)
	{
		Vector2 platePos(mPlateLeftTop.x, mPlateLeftTop.y + mHealthPlate->GetSize().y * i);

		mHealthPlate->Render(platePos, scale, texPos, mHealthPlate->GetSize());
		if (isSelectRender && mSelectIndex == i)
		{
			mSelect->Render(platePos - Vector2(SELECT_OFFSET, SELECT_OFFSET), scale, texPos, mSelect->GetSize());
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
