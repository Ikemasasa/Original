#include "ProductionValue.h"

#include "lib/Math.h"

#include "CameraManager.h"
#include "GameManager.h"
#include "Singleton.h"

ProductionValue::ProductionValue()
{
	mValueData.clear();
}

ProductionValue::~ProductionValue()
{
	mFont.Release();
	mValueData.clear();
}

void ProductionValue::Add(const int& value, const Vector3& pos, const Vector3& rgb)
{
	if (value <= 0) return;

	ValueData data;
	data.value = value;
	data.pos = pos;
	data.color = Vector4(rgb, 0.0f); // �ŏ������Ȃ��X�^�[�g
	data.alphaSign = 1; // ALPHA�͍ŏ� 0 ������v���X
	data.timer = 0.0f;
	data.sinfactor = SIN_FACTOR_INIT;
	mValueData.emplace_back(data);
}

void ProductionValue::DelayAdd(const int& value, const Vector3& pos, const Vector3& rgb, const int delayFrame)
{
	DelayData data;
	data.value = value;
	data.pos = pos;
	data.rgb = rgb;
	data.delayFrame = delayFrame;
	data.elapsedFrame = 0;
	mDelayData.emplace_back(data);
}

void ProductionValue::Initialize()
{
	mFont.Initialize(FONT_SIZE, FONT_WEIGHT);
}

void ProductionValue::Update()
{
	for (auto it = mValueData.begin(); it != mValueData.end();)
	{
		auto& data = *it;

		// ���W��ݒ�
		Vector2 scrPos = data.pos.WorldToScreen(Singleton<CameraManager>().GetInstance().GetView(), Singleton<CameraManager>().GetInstance().GetProj());
		scrPos.y -= sinf(data.sinfactor) * 30.0f;
		
		// alpha�̒l��ݒ�
		data.color.w += ALPHA_ADD * data.alphaSign;
		data.color.w = Math::Clamp01(data.color.w);

		// sinfactor�̒l��ݒ�
		data.sinfactor += SIN_FACTOR_ADD;
		data.sinfactor = Math::Min(Define::PI, data.sinfactor);

		// �t�H���g�Z�b�g
		mFont.RenderSetValue(data.value, scrPos, Vector2(0.5f, 0.0f), data.color);

		// �^�C�}�[��DISP_TIME�ȏ�ɂȂ�΁A�����𔽓]������(������alpha�l��������)
		if (data.timer >= DISP_TIME)
		{
			data.alphaSign = -data.alphaSign;

		}
		data.timer += GameManager::elapsedTime * data.alphaSign;


		// �A���t�@��0��菬�����Ȃ�����ValueData�폜
		if (data.color.w <= 0.0f)
		{
			it = mValueData.erase(it);
		}
		else ++it;
	}

	// �x���f�[�^����
	for (auto it = mDelayData.begin(); it != mDelayData.end();)
	{
		auto& data = *it;

		if (data.elapsedFrame < data.delayFrame)
		{
			// �o�߃t���[�����Z
			++data.elapsedFrame;
			++it; // �C�e���[�^��
		}
		else
		{
			// �ʏ�f�[�^�ɒǉ�
			Add(data.value, data.pos, data.rgb);

			// �x���f�[�^����폜
			it = mDelayData.erase(it);
		}
	}
}

void ProductionValue::Render()
{
	mFont.Render();
}
