#include "HealthGauge.h"

#include "CameraManager.h"
#include "Define.h"
#include "Singleton.h"

void HealthGauge::Initialize()
{
	// �摜��ǂݍ���
	mBaseBoard = std::make_unique<Sprite>(L"Data/Image/base_board.png");
	mGaugeBoard = std::make_unique<Sprite>(L"Data/Image/gauge_board.png");
	mGauge = std::make_unique<Sprite>(L"Data/Image/gauge.png");
	mGaugeSelcet = std::make_unique<Sprite>(L"Data/Image/gauge_select.png");
	mBuffIcons = std::make_unique<Sprite>(L"Data/Image/buff_icons.png");

	// �t�H���g������
	mFont.Initialize(FONT_SIZE, FONT_WEIGHT);

	// �������t���O��on
	mIsInitialize = true;
}

void HealthGauge::Set(const Status& status, const Vector2& pos, Center center, float color)
{
	// ���������Ă��Ȃ���Ώ���������
	if (!mIsInitialize)	Initialize();

	// �X�e�[�^�X�ۑ�
	SetStatusValue(status);
	
	// �Z���^�[����
	DecideCenter(center);

	// ���W�ۑ�
	mPos = pos;
	mRGB = color;

	// �t�H���g�Z�b�g
	RenderSetFont();
}

void HealthGauge::Set(const Status& status, const Vector3& pos, Center center, float color)
{
	// ���������Ă��Ȃ���Ώ���������
	if (!mIsInitialize)	Initialize();

	// �X�e�[�^�X�ۑ�
	SetStatusValue(status);

	// �Z���^�[����
	DecideCenter(center);

	// ���W�ۑ�
	Matrix view = Singleton<CameraManager>().GetInstance().GetView();
	Matrix proj = Singleton<CameraManager>().GetInstance().GetProj();
	mPos = pos.WorldToScreen(view, proj);
	mRGB = color;

	// �t�H���g�Z�b�g
	RenderSetFont();
}

//void HealthGauge::Render()
//{
//	float angle = 0.0f;
//	Vector4 color(mRGB, mRGB, mRGB, 1.0f);
//	Vector4 hpGaugeColor(0.5f * mRGB, 0.7f * mRGB, 0.3f * mRGB, 1.0f);
//	Vector4 mpGaugeColor(0.6f * mRGB, 0.3f * mRGB, 0.6f * mRGB, 1.0f);
//
//	// �S�̂̉摜��`��
//	Vector2 scale(SCALE_X, SCALE_Y);
//	mBaseBoard->Render(mPos, scale, Vector2::ZERO, mBaseBoard->GetSize(), mCenter, angle, color);
//	
//	// HP�o�[��`��
//	float x = mPos.x + GAUGE_OFFSET;
//	float y = mPos.y + GAUGE_FIRST_OFFSET_Y + GAUGE_OFFSET;
//	Vector2 size(mGaugeBoard->GetSize().x * mHPRate, mGaugeBoard->GetSize().y);
//	mGaugeBoard->Render(Vector2(x, y), scale, Vector2::ZERO, mGaugeBoard->GetSize(), mCenter, angle, color);
//	mGauge->Render(Vector2(x, y), scale, Vector2::ZERO, size, mCenter, 0.0f, hpGaugeColor);
//
//	// MP�o�[��`��
//	y = mPos.y + (mGaugeBoard->GetSize().y * SCALE_Y) + GAUGE_FIRST_OFFSET_Y + GAUGE_OFFSET;
//	size.x = mGaugeBoard->GetSize().x * mMPRate;
//	mGaugeBoard->Render(Vector2(x, y), scale, Vector2::ZERO, mGaugeBoard->GetSize(), mCenter, 0.0f, color);
//	mGauge->Render(Vector2(x, y), scale, Vector2::ZERO, size, mCenter, 0.0f, mpGaugeColor);
//
//	// �o�t�A�C�R����`��
//	scale = Vector2(BUFF_ICON_SCALE, BUFF_ICON_SCALE);
//	size = Vector2(BUFF_ICON_SIZE, BUFF_ICON_SIZE);
//	x = mPos.x - mCenter.x * SCALE_X + GAUGE_OFFSET;
//	y = mPos.y - mCenter.y * SCALE_Y;
//	int buffNum = 0;
//	for (int i = 0; i < BUFF_NUM; ++i)
//	{
//		if (mBuffFlags[i])
//		{
//			Vector2 pos(x, y);
//			Vector2 texPos(BUFF_ICON_SIZE * i, 0.0f);
//			mBuffIcons->Render(pos, scale, texPos, size, Vector2::ZERO);
//
//			x += BUFF_ICON_SCALE_SIZE;
//		}
//	}
//
//	// �t�H���g��`��
//	mFont.Render();
//}

void HealthGauge::Render(bool isSelectRender)
{
	float angle = 0.0f;
	Vector4 color(mRGB, mRGB, mRGB, 1.0f);
	Vector4 hpGaugeColor(0.5f * mRGB, 0.7f * mRGB, 0.3f * mRGB, 1.0f);
	Vector4 mpGaugeColor(0.6f * mRGB, 0.3f * mRGB, 0.6f * mRGB, 1.0f);

	// �S�̂̉摜��`��
	Vector2 scale(SCALE_X, SCALE_Y);
	mBaseBoard->Render(mPos, scale, Vector2::ZERO, mBaseBoard->GetSize(), mCenter, angle, color);

	// HP�o�[��`��
	float x = mPos.x + GAUGE_OFFSET;
	float y = mPos.y + GAUGE_FIRST_OFFSET_Y + GAUGE_OFFSET;
	Vector2 size(mGaugeBoard->GetSize().x * mHPRate, mGaugeBoard->GetSize().y);
	mGaugeBoard->Render(Vector2(x, y), scale, Vector2::ZERO, mGaugeBoard->GetSize(), mCenter, angle, color);
	mGauge->Render(Vector2(x, y), scale, Vector2::ZERO, size, mCenter, 0.0f, hpGaugeColor);

	// MP�o�[��`��
	y = mPos.y + (mGaugeBoard->GetSize().y * SCALE_Y) + GAUGE_FIRST_OFFSET_Y + GAUGE_OFFSET;
	size.x = mGaugeBoard->GetSize().x * mMPRate;
	mGaugeBoard->Render(Vector2(x, y), scale, Vector2::ZERO, mGaugeBoard->GetSize(), mCenter, 0.0f, color);
	mGauge->Render(Vector2(x, y), scale, Vector2::ZERO, size, mCenter, 0.0f, mpGaugeColor);

	// �o�t�A�C�R����`��
	scale = Vector2(BUFF_ICON_SCALE, BUFF_ICON_SCALE);
	size = Vector2(BUFF_ICON_SIZE, BUFF_ICON_SIZE);
	x = mPos.x - mCenter.x * SCALE_X + GAUGE_OFFSET;
	y = mPos.y - mCenter.y * SCALE_Y;
	int buffNum = 0;
	for (int i = 0; i < BUFF_NUM; ++i)
	{
		if (mBuffFlags[i])
		{
			Vector2 pos(x, y);
			Vector2 texPos(BUFF_ICON_SIZE * i, 0.0f);
			mBuffIcons->Render(pos, scale, texPos, size, Vector2::ZERO);

			x += BUFF_ICON_SCALE_SIZE;
		}
	}

	if (isSelectRender)
	{
		// �I���摜��\��
		RenderSelect();
	}

	// �t�H���g��`��
	mFont.Render();
}

void HealthGauge::RenderSelect()
{
	// �S�̂̉摜��`��
	Vector2 scale(SCALE_X, SCALE_Y);
	float angle = 0.0f;
	Vector4 color(mRGB, mRGB, mRGB, SELECT_GAUGE_ALPHA);
	mGaugeSelcet->Render(mPos, scale, Vector2::ZERO, mBaseBoard->GetSize(), mCenter, angle, color);
}

void HealthGauge::SetStatusValue(const Status& status)
{
	// �X�e�[�^�X�̒l��ۑ�
	mName = status.GetName();
	mCurrentHP = status.GetHP();
	mHPRate = static_cast<float>(mCurrentHP) / static_cast<float>(status.GetMaxHP());
	mCurrentMP = status.GetMP();
	mMPRate = static_cast<float>(mCurrentMP) / static_cast<float>(status.GetMaxMP());

	//�o�t��ۑ�
	for (int i = 0; i < BUFF_NUM; ++i) mBuffFlags[i] = false; // �������񃊃Z�b�g
	if (status.GetBuffAtkRate()   > 1.0f) mBuffFlags[ATK_UP] = true;
	if (status.GetDebuffAtkRate() < 1.0f) mBuffFlags[ATK_DOWN] = true;
	if (status.GetBuffDefRate()   > 1.0f) mBuffFlags[DEF_UP] = true;
	if (status.GetDebuffDefRate() < 1.0f) mBuffFlags[DEF_DOWN] = true;
}

void HealthGauge::DecideCenter(const Center& center)
{
	// �Z���^�[�ۑ�
	Vector2 size = mBaseBoard->GetSize();
	Vector2 hSize = size * 0.5f;
	switch (center)
	{
	case LEFTTOP:	  mCenter = Vector2(0.0f, 0.0f); break;
	case MIDTOP:	  mCenter = Vector2(hSize.x, 0.0f); break;
	case RIGHTTOP:	  mCenter = Vector2(size.x, 0.0f); break;
	case LEFTMID:	  mCenter = Vector2(0.0f, hSize.y); break;
	case MID:		  mCenter = hSize; break;
	case RIGHTMID:    mCenter = Vector2(size.x, hSize.y); break;
	case LEFTBOTTOM:  mCenter = Vector2(0.0f, size.y); break;
	case MIDBOTTOM:	  mCenter = Vector2(hSize.x, size.y); break;
	case RIGHTBOTTOM: mCenter = size; break;
	}
}

void HealthGauge::RenderSetFont()
{
	Vector2 baseSize = mBaseBoard->GetSize();
	Vector2 gaugeSize = mGaugeBoard->GetSize();

	// ���O�̃t�H���g
	float x = mPos.x - (mCenter.x * SCALE_X) + (baseSize.x * SCALE_X) - 5.0f;// 5.0f = �����l
	float y = mPos.y - (mCenter.y * SCALE_Y);
	mFont.RenderSet(mName.c_str(), Vector2(x, y), Vector2(1.0f, 0.0f), Define::FONT_COLOR);

	// HP�̃t�H���g
	x = mPos.x - (mCenter.x * SCALE_X) + HP_STR_OFFSET_X;
	y = mPos.y - (mCenter.y * SCALE_Y) + GAUGE_FIRST_OFFSET_Y;
	mFont.RenderSet(L"HP", Vector2(x, y), Vector2::ZERO);

	x = mPos.x - (mCenter.x * SCALE_X) + (baseSize.x * SCALE_X) - HP_STR_OFFSET_X * 2.0f;
	mFont.RenderSetValue(mCurrentHP, Vector2(x, y), Vector2(1.0f, 0.0f));

	// MP�̃t�H���g
	x = mPos.x - (mCenter.x * SCALE_X) + HP_STR_OFFSET_X;
	y = mPos.y - (mCenter.y * SCALE_Y) + GAUGE_FIRST_OFFSET_Y + GAUGE_OFFSET + (gaugeSize.y * SCALE_Y);
	mFont.RenderSet(L"MP", Vector2(x, y), Vector2::ZERO);

	x = mPos.x - (mCenter.x * SCALE_X) + (baseSize.x * SCALE_X) - HP_STR_OFFSET_X * 2.0f;
	mFont.RenderSetValue(mCurrentMP, Vector2(x, y), Vector2(1.0f, 0.0f));
}
