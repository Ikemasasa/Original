#include "CharacterHealth.h"

#include "lib/Input.h"
#include "lib/Sprite.h"

#include "DataBase.h"
#include "Define.h"
#include "KeyGuide.h"
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
}

void CharacterHealth::Update(const std::vector<Status>& statusArray, int activeNo)
{
	// �v���[�g�̕K�v���擾
	mBoardNum = statusArray.size();
	mActiveNo = activeNo;
	
	// �o�t�f�[�^���X�V
	mBuffData.resize(mBoardNum);
	mDebuffData.resize(mBoardNum);

	if (Input::GetButtonTrigger(0, Input::BUTTON::UP))   mSelectIndex = (mSelectIndex + (mBoardNum - 1)) % mBoardNum;
	if (Input::GetButtonTrigger(0, Input::BUTTON::DOWN)) mSelectIndex = (mSelectIndex + 1) % mBoardNum;

	// ���O, HP, MP�̃t�H���g�Z�b�g

	for (int i = 0; i < mBoardNum; ++i)
	{
		float colorMul = 0.5f;
		float subPosX = 0.0f;
		if (mActiveNo == -1) { colorMul = 1.0f; subPosX = 0.0f; }
		if (mActiveNo == i) { colorMul = 1.0f; subPosX = 30.0f; }


		float boardY = i * mHealthBoard->GetSize().y;
		float width = 0.0f;
		Vector2 pos = {};
		Vector2 center = {};

		Vector4 color = Define::FONT_COLOR;
		color.x *= colorMul;
		color.y *= colorMul;
		color.z *= colorMul;

		// ���O
		pos = Vector2(mBoardLeftTop.x + mHealthBoard->GetSize().x * 0.5f - subPosX, mBoardLeftTop.y + NAME_OFFSET_Y + boardY);
		center = Vector2(0.5f, 0.0f);
		mFont.RenderSet(statusArray[i].GetName().c_str(), pos, center, color);

		// �o�t�A�C�R��
		pos.x -= mFont.GetWidth(statusArray[i].GetName().c_str()) * 0.5f + (ICON_SCALESIZE + ARROW_SCALESIZE_X);
		float buffRates[2] = { statusArray[i].GetBuffAtkRate(), statusArray[i].GetBuffDefRate() };
		float DebuffRates[2] = { statusArray[i].GetDebuffAtkRate(), statusArray[i].GetDebuffDefRate() };

		for (int k = 0; k < 2; ++k)
		{
			// �o�t���������Ă�Ȃ�
			if (buffRates[k] > 1.0f)
			{
				BuffData data;
				data.buffIndex = k;
				data.pos = pos;
				mBuffData[i].push_back(data);

				// ���W���X�V
				pos.x -= (ICON_SCALESIZE + ARROW_SCALESIZE_X);
			}

			// �f�o�t���������Ă���Ȃ�
			if (DebuffRates[k] < 1.0f)
			{
				BuffData data;
				data.buffIndex = k;
				data.pos = pos;
				mDebuffData[i].push_back(data);

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
			pos.x = mBoardLeftTop.x + STATUS_OFFSET_X - subPosX;
			pos.y = mBoardLeftTop.y + STATUS_OFFSET_Y + (STATUS_ADD_Y * k) + boardY;

			// �X�e�[�^�X��
			mFont.RenderSet(statusName[k], pos, Vector2::ZERO, color);

			// ���݂̒l (cur)
			pos.x += CUR_OFFSET_X;
			mFont.RenderSetValue(statusValue[k], pos, center, color);

			// ��؂� (/)
			pos.x += mFont.GetWidthValue(statusValue[k]) + DELIM_OFFSET_X;
			mFont.RenderSet(L"/", pos, Vector2::ZERO, color);

			// �ő�̒l (max)
			pos.x += mFont.GetWidth(L"/") + DELIM_OFFSET_X;
			mFont.RenderSetValue(statusMax[k], pos, center, color);
		}
	}
}

void CharacterHealth::Render(bool isSelectRender, bool isFontClear)
{

	for (int i = 0; i < mBoardNum; ++i)
	{
		float colorMul = 0.5f;
		float subPosX = 0.0f;
		if (mActiveNo == -1) { colorMul = 1.0f; subPosX = 0.0f; }
		if (mActiveNo == i) { colorMul = 1.0f; subPosX = 30.0f; }

		Vector2 pos(mBoardLeftTop.x - subPosX, mBoardLeftTop.y + mHealthBoard->GetSize().y * i);
		Vector2 scale = Vector2::ONE;
		Vector2 texPos = Vector2::ZERO;
		Vector2 size = mHealthBoard->GetSize();
		Vector2 center = Vector2::ZERO;

		mHealthBoard->Render(pos, scale, texPos, size, center, 0.0f, Vector4(Vector3::ONE * colorMul, 1.0f));

		// �o�t�\��
		for (const auto& data : mBuffData[i])
		{
			// ���
			pos = Vector2(data.pos.x - subPosX, data.pos.y);
			scale  = Vector2(ARROW_SCALE, ARROW_SCALE);
			size   = Vector2(ARROW_SIZE_X, ARROW_SIZE_Y);
			texPos = Vector2(ARROW_TEXPOS_X, 0.0f);
			Vector4 color = Define::FONT_BUFF_COLOR * colorMul;
			color.w = 1.0f;
			mBuffIcon->Render(pos + Vector2(ICON_SCALESIZE, 0.0f), scale, texPos, size, center, 0.0f, color);

			// �A�C�R��
			scale  = Vector2(ICON_SCALE, ICON_SCALE);
			size   = Vector2(ICON_SIZE, ICON_SIZE);
			texPos = Vector2(data.buffIndex * ICON_SIZE, 0.0f);
			mBuffIcon->Render(pos, scale, texPos, size, Vector2::ZERO, 0.0f, Vector4(Vector3::ONE * colorMul, 1.0f));
		}

		// �f�o�t�\��
		for (const auto& data : mDebuffData[i])
		{
			// ���
			pos = Vector2(data.pos.x - subPosX, data.pos.y);
			scale = Vector2(ARROW_SCALE, ARROW_SCALE);
			size = Vector2(ARROW_SIZE_X, ARROW_SIZE_Y);
			texPos = Vector2(ARROW_TEXPOS_X, 0.0f);
			center = size / 2.0f;
			float angle = Define::PI; // �㉺���]������
			Vector4 color = Define::FONT_DEBUFF_COLOR * colorMul;
			color.w = 1.0f;
			mBuffIcon->Render(pos + Vector2(ICON_SCALESIZE + ARROW_SCALESIZE_X / 2.0f, ARROW_SCALESIZE_Y / 2.0f), scale, texPos, size, center, angle, color);

			// �A�C�R��
			scale = Vector2(ICON_SCALE, ICON_SCALE);
			size = Vector2(ICON_SIZE, ICON_SIZE);
			texPos = Vector2(data.buffIndex * ICON_SIZE, 0.0f);
			mBuffIcon->Render(pos, scale, texPos, size, Vector2::ZERO, 0.0f, Vector4(Vector3::ONE * colorMul, 1.0f));
		}
	}

	// �I���摜��`��
	if (isSelectRender)
	{
		Vector2 selectPos(mBoardLeftTop.x, mBoardLeftTop.y + mHealthBoard->GetSize().y * mSelectIndex);
		mSelect->Render(selectPos, Vector2::ONE, Vector2::ZERO, mSelect->GetSize());

		// �L�[�K�C�h
		KeyGuide::Instance().Add(KeyGuide::A, L"����");

		KeyGuide::Key key[] = {KeyGuide::UP, KeyGuide::DOWN };
		KeyGuide::Instance().Add(key, 2, L"�L�����N�^�[�I��");
	}

	// �t�H���g�N���A���Ȃ� = update��1�񂵂����Ȃ��A�Ȃ̂ł���
	if (isFontClear)
	{
		mBuffData.clear();
		mDebuffData.clear();
	}

	mFont.Render(isFontClear);
}

void CharacterHealth::Release()
{
	mFont.Release();
}

Vector2 CharacterHealth::GetBoardSize() const
{
	return mHealthBoard->GetSize();
}
