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
	mSelect = std::make_unique<Sprite>(L"Data/Image/Menu/health_select.png");
	mBuffIcon = std::make_unique<Sprite>(L"Data/Image/Battle/buff_icon.png");

	mBoardLeftTop = leftTop;

	mFont.Initialize();
	mFontValue.Initialize();
}

void CharacterHealth::Update(const std::vector<Status>& statusArray)
{
	// �v���[�g�̕K�v���擾
	mBoardNum = statusArray.size();
	
	// �o�t�f�[�^���X�V
	mBuffData.resize(mBoardNum);

	if (Input::GetButtonTrigger(0, Input::BUTTON::UP))   mSelectIndex = (mSelectIndex + (mBoardNum - 1)) % mBoardNum;
	if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mSelectIndex = (mSelectIndex + 1) % mBoardNum;

	// ���O, HP, MP�̃t�H���g�Z�b�g
	for (int i = 0; i < mBoardNum; ++i)
	{
		float boardY = i * mHealthBoard->GetSize().y;
		float width = 0.0f;
		Vector2 pos = {};
		Vector2 center = {};

		// ���O
		width = mFont.GetWidth(statusArray[i].GetName().c_str());
		pos = Vector2(mBoardLeftTop.x + mHealthBoard->GetSize().x * 0.5f, mBoardLeftTop.y + NAME_OFFSET_Y + boardY);
		center = Vector2(width * 0.5f, 0.0f);
		mFont.RenderSet(statusArray[i].GetName().c_str(), pos, center, Define::FONT_COLOR);

		// �o�t�A�C�R��
		pos.x -= width * 0.5f + (ICON_SCALESIZE + ARROW_SCALESIZE_X);
		float rates[2] = { statusArray[i].GetBuffAtkRate(), statusArray[i].GetBuffDefRate() };

		for (int k = 0; k < 2; ++k)
		{
			// �o�t���������Ă�Ȃ�
			if (rates[k] > 1.0f)
			{
				BuffData data;
				data.buffIndex = k;
				data.pos = pos;
				mBuffData[i].push_back(data);

				// ���W���X�V
				pos.x -= (ICON_SCALESIZE + ARROW_SCALESIZE_X);
			}
		}


		// �X�e�[�^�X���l��z��
		const wchar_t* statusName[STATUS_NUM] = { L"HP", L"MP" };
		int statusValue[STATUS_NUM] = { statusArray[i].GetHP(), statusArray[i].GetMP() };
		int statusMax[STATUS_NUM] = { statusArray[i].GetMaxHP(), statusArray[i].GetMaxMP() };
		center = Vector2::ZERO;
		for (int k = 0; k < STATUS_NUM; ++k)
		{
			// �n�߂̍��W�ݒ�
			pos.x = mBoardLeftTop.x + STATUS_OFFSET_X;
			pos.y = mBoardLeftTop.y + STATUS_OFFSET_Y + (STATUS_ADD_Y * k) + boardY;

			// �X�e�[�^�X��
			mFont.RenderSet(statusName[k], pos, Vector2::ZERO, Define::FONT_COLOR);

			// ���݂̒l (cur)
			pos.x += CUR_OFFSET_X;
			mFontValue.RenderSet(statusValue[k], pos, center, Define::FONT_COLOR);

			// ��؂� (/)
			pos.x += mFontValue.GetWidth(statusValue[k]) + DELIM_OFFSET_X;
			mFont.RenderSet(L"/", pos, Vector2::ZERO, Define::FONT_COLOR);

			// �ő�̒l (max)
			pos.x += mFont.GetWidth(L"/") + DELIM_OFFSET_X;
			mFontValue.RenderSet(statusMax[k], pos, center, Define::FONT_COLOR);
		}
	}
}

void CharacterHealth::Render(bool isSelectRender, bool isFontClear)
{

	for (int i = 0; i < mBoardNum; ++i)
	{
		Vector2 pos(mBoardLeftTop.x, mBoardLeftTop.y + mHealthBoard->GetSize().y * i);
		Vector2 scale = Vector2::ONE;
		Vector2 texPos = Vector2::ZERO;
		Vector2 size = mHealthBoard->GetSize();
		Vector2 center = Vector2::ZERO;

		mHealthBoard->Render(pos, scale, texPos, size);

		for (auto data : mBuffData[i])
		{
			// ���
			scale  = Vector2(ARROW_SCALE, ARROW_SCALE);
			size   = Vector2(ARROW_SIZE_X, ARROW_SIZE_Y);
			texPos = Vector2(ARROW_TEXPOS_X, 0.0f);
			mBuffIcon->Render(data.pos + Vector2(ICON_SCALESIZE, 0.0f), scale, texPos, size, center, 0.0f, Define::FONT_BUFF_COLOR);

			// �A�C�R��
			scale  = Vector2(ICON_SCALE, ICON_SCALE);
			size   = Vector2(ICON_SIZE, ICON_SIZE);
			texPos = Vector2(data.buffIndex * ICON_SIZE, 0.0f);
			mBuffIcon->Render(data.pos, scale, texPos, size);
		}
	}

	// �I���摜��`��
	if (isSelectRender)
	{
		Vector2 selectPos(mBoardLeftTop.x, mBoardLeftTop.y + mHealthBoard->GetSize().y * mSelectIndex);
		mSelect->Render(selectPos, Vector2::ONE, Vector2::ZERO, mSelect->GetSize());
	}

	// �t�H���g�N���A���Ȃ� = update��1�񂵂����Ȃ��A�Ȃ̂ł���
	if(isFontClear)mBuffData.clear();

	mFont.Render(isFontClear);
	mFontValue.Render(isFontClear);
}

void CharacterHealth::Release()
{
	mFont.Release();
	mFontValue.Release();
}
