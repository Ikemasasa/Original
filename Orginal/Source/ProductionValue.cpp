#include "ProductionValue.h"

#include "lib/Math.h"

#include "CameraManager.h"
#include "Singleton.h"

ProductionValue::ProductionValue()
{
	mValueData.clear();
}

ProductionValue::~ProductionValue()
{
	mFontValue.Release();
	mValueData.clear();
}

void ProductionValue::Add(const int& value, const Vector3& pos, const Vector3& rgb)
{
	ValueData data;
	data.value = value;
	data.pos = pos;
	data.scale = Vector2::ONE; // �X�P�[����1�Œ�
	data.center = Vector2(mFontValue.GetWidth(data.value) / 2.0f, 0.0f); // x���� y��ԏ�
	data.color = Vector4(rgb.x, rgb.y, rgb.z, 0.0f); // �ŏ������Ȃ��X�^�[�g
	data.alphaSign = 1; // ALPHA�͍ŏ� 0 ������v���X

	mValueData.emplace_back(data);
}

void ProductionValue::Initialize()
{
	mFontValue.Initialize(FONT_SIZE, FONT_WEIGHT);
}

void ProductionValue::Update()
{
	const float POS_Y_MOVE = 0.01f; // ������Y���W�𓮂����悤
	const float ALPHA_ADD = 0.05f;	// ALPHA�̑����l
	const float SIGN_CHANGE_VALUE = 1.8f; // �A���t�@���܂�Ԃ��l

	for (auto it = mValueData.begin(); it != mValueData.end();)
	{
		auto& data = *it;

		// ���X�ɏ�Ɉړ�
		data.pos.y += POS_Y_MOVE;
		Vector2 scrPos = data.pos.WorldToScreen(Singleton<CameraManager>().GetInstance().GetView(), Singleton<CameraManager>().GetInstance().GetProj());
		
		// alpha�̒l�𑝌�������
		data.color.w = data.color.w + ALPHA_ADD * data.alphaSign;

		mFontValue.RenderSet(data.value, scrPos, data.center, data.scale, data.color);

		// �A���t�@��SIGN_CHANGE_VALUE�ȏ�ɂȂ�΁A�����𔽓]������(������alpha�l��������)
		if (data.color.w >= SIGN_CHANGE_VALUE)
		{
			data.alphaSign = -data.alphaSign; 
		}

		// �A���t�@��0��菬�����Ȃ�����ValueData�폜
		if (data.color.w <= 0.0f)
		{
			it = mValueData.erase(it);
		}
		else ++it;
	}
}

void ProductionValue::Render()
{
	mFontValue.Render();
}
